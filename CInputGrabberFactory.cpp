#include "CInputGrabberFactory.h"
#include "CInputGrabberWin.h"
#include "CInputGrabberLinux.h"
#include <string.h>

IInputGrabberIn* CInputGrabberFactory::theInstance = NULL;

IInputGrabberIn* CInputGrabberFactory::getInstance(){
	return theInstance;
}

IInputGrabberIn* CInputGrabberFactory::CreateInstance(const char* exeName, IInputGrabberOut* itsOwner){
#ifdef _WIN32
	theInstance = new CInputGrabberWin(exeName, itsOwner);
#elif __linux__
	theInstance = new CInputGrabberLinux(itsOwner);
#endif
	return theInstance;
}
