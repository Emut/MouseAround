
//#ifdef __linux__
#include "CInputGrabberLinux.h"
#include <iostream>
using namespace std;

CInputGrabberLinux::CInputGrabberLinux(IInputGrabberOut* itsOwner){
	m_itsOwner = itsOwner;
}

bool CInputGrabberLinux::setMousePos(int nPosX, int nPosY){
	cout<<"setMousePos"<<nPosX<<' '<<nPosY<<endl;
	return false;
}
bool CInputGrabberLinux::getMousePos(int* nPosX, int* nPosY){
	cout<<"getMousePos"<<endl;
	return false;
}
bool CInputGrabberLinux::InsertKeyPress(unsigned char key, bool isPressed){
	cout<<"InsertKeyPress"<<key<<' '<<isPressed<<endl;
	return false;
}

bool CInputGrabberLinux::InsertMouseWheel(int deltaX, int deltaY){
	cout<<"InsertMouseWheel"<<deltaX<<' '<<deltaY<<endl;
	return false;
}
//#endif