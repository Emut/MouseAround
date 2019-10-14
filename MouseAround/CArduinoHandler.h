#pragma once
#include "IScreenHandler.h"
#include <mutex>
#include <Windows.h>

class CArduinoHandler :
	public IScreenHandlerIn
{
	std::mutex mtxWriteLock;
	HANDLE hndComm;
public:
	void SendRelativeMouseInput(int nDelX, int nDelY, bool bIsWheel);
	void SendExactMouseInput(int nX, int nY);
	void SendKeyboardInput(unsigned char ucKey, bool bIsPressed);
	CArduinoHandler(int nComID);
	~CArduinoHandler();
};

