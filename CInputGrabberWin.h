#pragma once

#ifdef _WIN32
#include <Windows.h>


#include "IInputGrabber.h"

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
	bool InsertKeyPress(unsigned char key, bool isPressed);
	bool InsertMouseWheel(int deltaX, int deltaY);
};

#endif
