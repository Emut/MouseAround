#pragma once
#include "IScreenHandler.h"
#include <mutex>
#include "CSerial.h"

class CArduinoHandler :
	public IScreenHandlerIn
{
	std::mutex mtxWriteLock;
	unsigned char ucMessageID;
	int nScreenPosX;
	int nScreenPosY;
	int nScreenSizeX;
	int nScreenSizeY;
	CSerial itsComm;
public:
	void SendRelativeMouseInput(int nDelX, int nDelY, bool bIsWheel);
	void SendExactMouseInput(int nX, int nY);
	void SendKeyboardInput(unsigned char ucKey, bool bIsPressed);
	void getMousePos(int& nX, int& nY);
	CArduinoHandler(int nComID, int width, int heigth);
	~CArduinoHandler();
};

