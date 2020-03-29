#include <stdio.h>
#include "CNetworkHostHandler.h"

CNetworkHostHandler::CNetworkHostHandler(int width, int heigth, const char* destAddress, unsigned short destPort, const char* sourceAddress, unsigned short sourcePort){
	itsSocket = new CSocket(destPort, destAddress, sourcePort, sourceAddress);
	xPos = 0;
	yPos = 0;
	m_width = width;
	m_heigth = heigth;
}


void CNetworkHostHandler::SendRelativeMouseInput(int nDelX, int nDelY, bool bIsWheel){

	if(!bIsWheel){
		xPos += nDelX;
		yPos += nDelY;
	}
	const int msgLen = 11;
	unsigned char buf[msgLen];
	if(!bIsWheel)
		buf[0] = 0xC3;
	else
		buf[0] = 0xC5;
	buf[1] = 0;
	
	buf[2] = (nDelX>>24)&0xFF;
	buf[3] = (nDelX>>16)&0xFF;
	buf[4] = (nDelX>> 8)&0xFF;
	buf[5] = (nDelX    )&0xFF;

	buf[6] = (nDelY>>24)&0xFF;
	buf[7] = (nDelY>>16)&0xFF;
	buf[8] = (nDelY>> 8)&0xFF;
	buf[9] = (nDelY    )&0xFF;

	unsigned char checksum = 0;
	for(int i = 0; i < msgLen-1; ++i){
		checksum =+ buf[i];
	}
	buf[10] = checksum;

	itsSocket->Send(buf, msgLen);
}

void CNetworkHostHandler::SendExactMouseInput(int nX, int nY){

	xPos = nX;
	yPos = nY;
	const int msgLen = 11;
	unsigned char buf[msgLen];
	buf[0] = 0xC4;
	buf[1] = 0;
	
	buf[2] = (nX>>24)&0xFF;
	buf[3] = (nX>>16)&0xFF;
	buf[4] = (nX>> 8)&0xFF;
	buf[5] = (nX    )&0xFF;

	buf[6] = (nY>>24)&0xFF;
	buf[7] = (nY>>16)&0xFF;
	buf[8] = (nY>> 8)&0xFF;
	buf[9] = (nY    )&0xFF;

	unsigned char checksum = 0;
	for(int i = 0; i < msgLen-1; ++i){
		checksum =+ buf[i];
	}
	buf[10] = checksum;

	itsSocket->Send(buf, msgLen);
	//Sleep(1000);
}

void CNetworkHostHandler::SendKeyboardInput(unsigned char ucKey, bool bIsPressed){
	//printf("CNetworkHostHandler::SendKeyboardInput\n");
	
	const int msgLen = 5;
	unsigned char buf[msgLen];
	buf[0] = bIsPressed?0xC1:0xC2;
	buf[1] = 0;
	
	buf[2] = ucKey;
	buf[3] = 0;
	

	unsigned char checksum = 0;
	for(int i = 0; i < msgLen-1; ++i){
		checksum =+ buf[i];
	}
	buf[msgLen-1] = checksum;

	itsSocket->Send(buf, msgLen);
	
	
	//INPUT myInput;
	//myInput.type = INPUT_KEYBOARD;
	//myInput.ki.wVk = ucKey;
	//if(!bIsPressed)
	//	myInput.ki.dwFlags = KEYEVENTF_KEYUP;
	//else
	//	myInput.ki.dwFlags = 0;

	//SendInput(1, &myInput, sizeof(myInput));
	//Sleep(1);
	
}

void CNetworkHostHandler::getMousePos(int& nX, int& nY){
	nX = xPos;
	nY = yPos;
}