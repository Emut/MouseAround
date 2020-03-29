#pragma once
#include "IInputGrabber.h"

class CInputGrabberFactory{
public:
	static IInputGrabberIn* CreateInstance(const char*, IInputGrabberOut*);
	static IInputGrabberIn* getInstance();
private:
	static IInputGrabberIn* theInstance;
};