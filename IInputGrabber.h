#pragma once

class IInputGrabberOut
{
public:
	virtual bool MouseUpdated(int nPosX, int nPosY, int nDelX, int nDelY, bool bIsWheel) = 0;
	virtual bool KeyboardUpdated(unsigned char ucKey, bool bIsPressed) = 0;
};

class IInputGrabberIn
{
public:
	virtual bool setMousePos(int nPosX, int nPosY) = 0;
	virtual bool getMousePos(int* nPosX, int* nPosY) = 0;
	virtual bool InsertKeyPress(unsigned char key, bool isPressed) = 0;
	virtual bool InsertMouseWheel(int deltaX, int deltaY) = 0;
};