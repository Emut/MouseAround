#include "CArduinoHandler.h"
#include "KeyboardTypeHelpers.h"
#include <thread>

//#ifndef MIN(a,b)

#define MIN(a,b) ((a)<(b)?(a):(b))

//#endif

//#ifndef MAX(a,b)

#define MAX(a,b) ((a)>(b)?(a):(b))

// #endif

CArduinoHandler::CArduinoHandler(int nComID, int width, int heigth): itsComm(nComID) {
	ucMessageID = 0;

	for(int i = 0; i < 20; ++i)
		SendRelativeMouseInput(-100,-100,false);

	nScreenPosX = 0;
	nScreenPosY = 0;
	nScreenSizeX = width;
	nScreenSizeY = heigth;
}

void CArduinoHandler::SendRelativeMouseInput(int nDelX, int nDelY, bool bIsWheel) {
	
	if(nDelX == 0 && nDelY == 0)
		return;
	mtxWriteLock.lock();

	if (nDelX > 127)
			nDelX = 127;
		if (nDelY > 127)
			nDelY = 127;
		if (nDelX < -127)
			nDelX = -127;
		if (nDelY < -127)
			nDelY = -127;

	

	//printf("%d,%d\n", nScreenPosX, nScreenPosY);
	
	//std::this_thread::sleep_for(std::chrono::microseconds(500));
	std::this_thread::sleep_for(std::chrono::milliseconds(1)); //non controlled succesive messages sometimes get ignored. Bundle mtxLock and delay in a function? Or a smart res holder?
	unsigned char cpBuffer[5];
	if (!bIsWheel) {
		//printf("%d,%d...\n", nDelX, nDelY);
		nScreenPosX+=nDelX;
		nScreenPosY+=nDelY;
		if(nScreenPosX<0)
			nScreenPosX=0;
		if(nScreenPosX>nScreenSizeX-1)
			nScreenPosX=nScreenSizeX-1;
		if(nScreenPosY<0)
			nScreenPosY=0;
		if(nScreenPosY>nScreenSizeY-1)
			nScreenPosY=nScreenSizeY-1;
		
		cpBuffer[0] = 0xC3;
		cpBuffer[1] = ucMessageID;
		cpBuffer[2] = (unsigned char)nDelX;
		cpBuffer[3] = (unsigned char)nDelY;
		unsigned char ucCheckSum = 0;
		ucCheckSum += cpBuffer[0];
		ucCheckSum += cpBuffer[1];
		ucCheckSum += cpBuffer[2];
		ucCheckSum += cpBuffer[3];
		cpBuffer[4] = ucCheckSum;
	}
	else {
		cpBuffer[0] = 0xC5;
		cpBuffer[1] = ucMessageID;
		cpBuffer[2] = (unsigned char)(nDelX / 120);
		cpBuffer[3] = 0;
		unsigned char ucCheckSum = 0;
		ucCheckSum += cpBuffer[0];
		ucCheckSum += cpBuffer[1];
		ucCheckSum += cpBuffer[2];
		ucCheckSum += cpBuffer[3];
		cpBuffer[4] = ucCheckSum;
	}

	itsComm.Send(cpBuffer, 5);
	itsComm.Send(cpBuffer, 5);
	++ucMessageID;
	mtxWriteLock.unlock();
}
void CArduinoHandler::SendExactMouseInput(int nX, int nY) {
	//printf("\nExact to %d %d", nX, nY);
	int targetX = nX<nScreenSizeX/2 ? -100 : nScreenSizeX+100;
	int targetY = nY<nScreenSizeY/2 ? -100 : nScreenSizeY+100;

	int tempX = nScreenPosX;
	int tempY = nScreenPosY;
	//go into the closest corner
	while(tempX != targetX || tempY != targetY){
		//printf("\n1:%d,%d,%d,%d", tempX, tempY, targetX, targetY);
		int nActionX = targetX - tempX;
		int nActionY = targetY - tempY;

		nActionX = MIN(nActionX, 127);
		nActionX = MAX(nActionX, -127);

		nActionY = MIN(nActionY, 127);
		nActionY = MAX(nActionY, -127);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		SendRelativeMouseInput(nActionX, nActionY, false);

		tempX += nActionX;
		tempY += nActionY;
	}
	//now go into the target location
	targetX = nX;
	targetY = nY;

	tempX = nScreenPosX;
	tempY = nScreenPosY;

	while(tempX != targetX || tempY != targetY){
		//printf("\n2:%d,%d,%d,%d", tempX, tempY, targetX, targetY);
		int nActionX = targetX - tempX;
		int nActionY = targetY - tempY;

		nActionX = MIN(nActionX, 127);
		nActionX = MAX(nActionX, -127);

		nActionY = MIN(nActionY, 127);
		nActionY = MAX(nActionY, -127);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		SendRelativeMouseInput(nActionX, nActionY, false);

		tempX += nActionX;
		tempY += nActionY;
	}

	
	

}
void CArduinoHandler::SendKeyboardInput(unsigned char ucKey, bool bIsPressed) {
	mtxWriteLock.lock();
	unsigned char ucTemp = ucKey;
	if (!KeyboardTypeHelper::WinToArduino(ucKey)) {
		mtxWriteLock.unlock();
		return;			//input key is not applicable for arduino
	}

	unsigned char cpBuffer[5];
	if (bIsPressed)
		cpBuffer[0] = 0xC1;
	else
		cpBuffer[0] = 0xC2;
	cpBuffer[1] = ucMessageID;
	cpBuffer[2] = ucKey;
	cpBuffer[3] = 0;
	unsigned char ucCheckSum = 0;
	ucCheckSum += cpBuffer[0];
	ucCheckSum += cpBuffer[1];
	ucCheckSum += cpBuffer[2];
	ucCheckSum += cpBuffer[3];
	cpBuffer[4] = ucCheckSum;


	itsComm.Send(cpBuffer, 5);
	itsComm.Send(cpBuffer, 5);
	++ucMessageID;
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	mtxWriteLock.unlock();
}

void CArduinoHandler::getMousePos(int& nX, int& nY){
	nX = nScreenPosX;
	nY = nScreenPosY;
}

CArduinoHandler::~CArduinoHandler() {
	
}
