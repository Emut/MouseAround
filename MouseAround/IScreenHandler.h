#pragma once

class IScreenHandlerIn {
public:
	virtual void SendRelativeMouseInput(int nDelX, int nDelY, bool bIsWheel) = 0;
	virtual void SendExactMouseInput(int nX, int nY) = 0;
	virtual void SendKeyboardInput(unsigned char ucKey, bool bIsPressed) = 0;
};