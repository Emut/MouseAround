#pragma once
#include <Windows.h>


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
};

class CInputGrabberWin: public IInputGrabberIn
{
	POINT prevMousePos;
	IInputGrabberOut* itsOwner;
public:

	LRESULT MouseCallback(int nCode, WPARAM wParam, LPARAM lParam);
	LRESULT KeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam);
	HHOOK hMouseHook;
	HHOOK hKeyboardHook;
	CInputGrabberWin(const char*, IInputGrabberOut*);
	~CInputGrabberWin();
	bool setMousePos(int nPosX, int nPosY);
	bool getMousePos(int* nPosX, int* nPosY);

};

