#pragma once

//#ifdef __linux__
#include "IInputGrabber.h"

class CInputGrabberLinux: public IInputGrabberIn
{
public:
	CInputGrabberLinux(IInputGrabberOut* itsOwner);

	bool setMousePos(int nPosX, int nPosY);
	bool getMousePos(int* nPosX, int* nPosY);
	bool InsertKeyPress(unsigned char key, bool isPressed);
	bool InsertMouseWheel(int deltaX, int deltaY);
private:
	IInputGrabberOut* m_itsOwner;
};

//#endif