#include "CSelfHandler.h"
#include <list>
#include <iostream>

CSelfHandler::CSelfHandler(unsigned short port){
	itsInputGrabber = CInputGrabberFactory::getInstance();
	itsSocket = new CSocket(0, NULL, 12357, NULL);
	threadSocketListener = new std::thread(&CSelfHandler::SocketListener, this);
	threadCaller = new std::thread(&CSelfHandler::ActiveFunction, this);
}

void CSelfHandler::SendRelativeMouseInput(int nDelX, int nDelY, bool bIsWheel){
	if(bIsWheel){
		wheelCall = true;
		m_wheelDeltaX = nDelX;
		m_wheelDeltaY = nDelY;
	}
	else{
		int x,y;
		itsInputGrabber->getMousePos(&x, &y);

		nExactPosInputX = x + nDelX;
		nExactPosInputY = y + nDelY;
		mouseCall = true;
	}
	sem.Signal();
}
void CSelfHandler::SendExactMouseInput(int nX, int nY){
	nExactPosInputX = nX;
	nExactPosInputY = nY;
	mouseCall = true;
	sem.Signal();
}
void CSelfHandler::SendKeyboardInput(unsigned char ucKey, bool bIsPressed){
	//printf("CSelfHandler::SendKeyboardInput %d\n", ucKey);
	key = ucKey;
	isPressed = bIsPressed;
	keyCall = true;
	sem.Signal();
}
void CSelfHandler::getMousePos(int& nX, int& nY){
	itsInputGrabber->getMousePos(&nX, &nY);
}

void CSelfHandler::ActiveFunction(){
	
	while(true){
		sem.Wait();
		//printf("CSelfHandler::ActiveFunctionMain\n");
		//printf("%d,%d\n", nExactPosInputX, nExactPosInputY);
		if(keyCall){
			//printf("CSelfHandler::ActiveFunctionKey\n", key);
			itsInputGrabber->InsertKeyPress(key, isPressed);
			keyCall = false;
		}
		if(mouseCall){
			//printf("CSelfHandler::ActiveFunctionMouse\n");
			itsInputGrabber->setMousePos(nExactPosInputX, nExactPosInputY);
			mouseCall = false;
		}
		if(wheelCall){
			itsInputGrabber->InsertMouseWheel(m_wheelDeltaX, m_wheelDeltaY);
			wheelCall = false;
		}
		
	}
}

void CSelfHandler::SocketListener(){
	std::list<unsigned char> lst;
	while(true){
		const int bufSize = 100;
		char buf[bufSize];
		auto readCount = itsSocket->Read(buf, bufSize);

		for(int i = 0; i < readCount; ++i){
			lst.push_back(buf[i]);
		}

		//for(const auto it: lst)
		//	std::cout<<(int)it<<' ';
		//std::cout<<std::endl;

		while(lst.size() > 4){	//min msg size
			switch(lst.front()){
			case 0xC4:	//exact input case
			case 0xC3:	//relative input case
			case 0xC5:	//mouse wheel case
			{
				if(lst.size() < 10)
					break;	//mouse msg is longer. leave until more msg arrives.

				unsigned char header = lst.front();
				lst.pop_front(); //pop the header
				unsigned char msgID = 0;
				int nX = 0;
				int nY = 0;
				unsigned char checksum = header; 
				unsigned char calculatedCS = 0;

				auto it = lst.cbegin();
				msgID = *(it++);
				calculatedCS += msgID;
				for(int i = 0; i < 4; ++i){
					calculatedCS += *it;
					nX <<= 8;
					nX += *(it++);
				}
				for(int i = 0; i < 4; ++i){
					calculatedCS += *it;
					nY <<= 8;
					nY += *(it++);
				}
				calculatedCS += *it;
				checksum = *it;

				//if(calculatedCS != checksum){		//commented out for tests
				//	//no need to pop anything else. Header byte is already poped
				//	continue;
				//}
				for(int i = 0; i < 10; ++i){
					//pop the entire msg
					lst.pop_front();
				}	
				if(header == 0xC4){
					//printf("SelfHandler::SendExactMouseInput(%d,%d)", nX, nY);
					SendExactMouseInput(nX, nY);
				}
				else if(header == 0xC3)
					SendRelativeMouseInput(nX, nY, false);
				else if(header == 0xC5)
					SendRelativeMouseInput(nX, nY, true);
			}
				break;

			case 0xC1:
			case 0xC2:
				{
					unsigned char header = lst.front();
					lst.pop_front(); //pop the header
					unsigned char msgID = 0;
					unsigned char key = 0;
					unsigned char reserved = 0;
					unsigned char checksum = header; 
					unsigned char calculatedCS = 0;

					auto it = lst.cbegin();
					msgID = *(it++);
					calculatedCS += msgID;

					key = *(it++);
					calculatedCS += msgID;

					reserved = *(it++);
					calculatedCS += msgID;

					checksum = *(it++);
					calculatedCS += msgID;

					//if(calculatedCS != checksum){		//commented out for tests
					//	//no need to pop anything else. Header byte is already poped
					//	continue;
					//}
					for(int i = 0; i < 4; ++i){
						//pop the entire msg
						lst.pop_front();
					}	
					if(header == 0xC1)
						SendKeyboardInput(key, true);
					else if(header == 0xC2)
						SendKeyboardInput(key, false);
				}
				
				break;
			default:
				lst.pop_front(); //not the header
				break;
			}
		}

	}
}
