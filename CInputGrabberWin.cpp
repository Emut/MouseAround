#ifdef _WIN32
#include "CInputGrabberWin.h"
#include <stdio.h>
#include <windows.h>

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#pragma comment(lib, "user32.lib")

CInputGrabberWin* callbackInstance = 0;

void MessageLoop() {
	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}


__declspec(dllexport) LRESULT CALLBACK MouseEvent(int nCode, WPARAM wParam, LPARAM lParam) {
	LRESULT retVal = callbackInstance->MouseCallback(nCode, wParam, lParam);
	if (retVal == -1)
		return -1;
	return CallNextHookEx(callbackInstance->hMouseHook, nCode, wParam, lParam);
}

DWORD WINAPI MouseLogger(LPVOID lpParam) {
	HINSTANCE hInstance = GetModuleHandle(NULL);
	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParam);
	if (!hInstance) return 1;

	callbackInstance->hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseEvent, hInstance, NULL);
	MessageLoop();
	UnhookWindowsHookEx(callbackInstance->hMouseHook);
	return 0;
}

__declspec(dllexport) LRESULT CALLBACK KeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam) {
	LRESULT retVal = callbackInstance->KeyboardCallback(nCode, wParam, lParam);
	if (retVal == -1)
		return -1;
	return CallNextHookEx(callbackInstance->hKeyboardHook, nCode, wParam, lParam);
}

DWORD WINAPI KeyboardLogger(LPVOID lpParam) {
	HINSTANCE hInstance = GetModuleHandle(NULL);
	if (!hInstance) hInstance = LoadLibrary((LPCSTR)lpParam);
	if (!hInstance) return 1;

	callbackInstance->hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEvent, hInstance, NULL);
	MessageLoop();
	UnhookWindowsHookEx(callbackInstance->hKeyboardHook);
	return 0;
}


CInputGrabberWin::CInputGrabberWin(const char* cpExecName, IInputGrabberOut* par_itsOwner) {
	//printf("CInputGrabberWin::Constructor\n");
	itsOwner = par_itsOwner;
	callbackInstance = this;
	GetCursorPos(&prevMousePos);

	void* vpParams[2];
	vpParams[0] = (void*)cpExecName;
	vpParams[1] = (void*)this;
	HANDLE hMouseThread;
	DWORD dwMouseThread;
	hMouseThread = CreateThread(NULL, NULL,
		(LPTHREAD_START_ROUTINE)MouseLogger, (LPVOID)vpParams, NULL, &dwMouseThread);

	HANDLE hKbThread;
	DWORD dwKbThread;
	hKbThread = CreateThread(NULL, NULL,
		(LPTHREAD_START_ROUTINE)KeyboardLogger, (LPVOID)vpParams, NULL, &dwKbThread);
}

CInputGrabberWin::~CInputGrabberWin() {
	printf("CInputGrabberWin::Destructor\n");
}

LRESULT CInputGrabberWin::MouseCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	MOUSEHOOKSTRUCT* pMouseStruct = (MOUSEHOOKSTRUCT*)lParam;
	switch (wParam)
	{
	case WM_LBUTTONDOWN:
		if (itsOwner->KeyboardUpdated(VK_LBUTTON, true))
			return -1;
		else
			return 0;
		break;
	case WM_LBUTTONUP:
		if (itsOwner->KeyboardUpdated(VK_LBUTTON, false))
			return -1;
		else
			return 0;
		break;
	case WM_RBUTTONDOWN:
		if (itsOwner->KeyboardUpdated(VK_RBUTTON, true))
			return -1;
		else
			return 0;
		break;
	case WM_RBUTTONUP:
		if (itsOwner->KeyboardUpdated(VK_RBUTTON, false))
			return -1;
		else
			return 0;
		break;
	case WM_MBUTTONDOWN:
		if (itsOwner->KeyboardUpdated(VK_MBUTTON, true))
			return -1;
		else
			return 0;
		break;
	case WM_MBUTTONUP:
		if (itsOwner->KeyboardUpdated(VK_MBUTTON, false))
			return -1;
		else
			return 0;
		break;
	default:
		break;
	}
	
	if (pMouseStruct != NULL && wParam == WM_MOUSEMOVE) {
		int delX = pMouseStruct->pt.x - prevMousePos.x;
		int delY = pMouseStruct->pt.y - prevMousePos.y;
		
		if (itsOwner->MouseUpdated(pMouseStruct->pt.x, pMouseStruct->pt.y, delX, delY, false))
			return -1;
		else{
			prevMousePos = pMouseStruct->pt;
			return 0;
		}
	}

	if (pMouseStruct != NULL && wParam == WM_MOUSEWHEEL) {
		MSLLHOOKSTRUCT* temp = (MSLLHOOKSTRUCT*)pMouseStruct;
		short delta = temp->mouseData >> 16;
		//printf("wheel:%d\n", delta);
		if (itsOwner->MouseUpdated(pMouseStruct->pt.x, pMouseStruct->pt.y, delta, 0, true))
			return -1;
		else {
			prevMousePos = pMouseStruct->pt;
			return 0;
		}
		//pMouseStruct->hwnd
	}

	return 0;
}

LRESULT CInputGrabberWin::KeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT* pKbStruct = (KBDLLHOOKSTRUCT*)lParam;
	if(itsOwner->KeyboardUpdated((unsigned char)pKbStruct->vkCode, wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
		return -1;
	return 0;
}

bool CInputGrabberWin::setMousePos(int nPosX, int nPosY) {
	prevMousePos.x = nPosX;
	prevMousePos.y = nPosY;
	return SetCursorPos(nPosX, nPosY) == TRUE;
}

bool CInputGrabberWin::getMousePos(int* nPosX, int* nPosY) {
	POINT ptTemp;
	GetCursorPos(&ptTemp);
	*nPosX = ptTemp.x;
	*nPosY = ptTemp.y;
	return true;
}

bool CInputGrabberWin::InsertKeyPress(unsigned char key, bool isPressed){
	//printf("CInputGrabberWin::InsertKeyPress %d", key);
	INPUT myInput;
	if(key == 1 || key == 2 || key == 4){
		myInput.mi.time = 0;
		myInput.mi.dx = 0; //rel movement since last event
		myInput.mi.dy = 0; 
		
		myInput.type = INPUT_MOUSE;
		if(key == 1 && isPressed)
			myInput.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		else if(key == 1 && !isPressed)
			myInput.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		else if(key == 2 && isPressed)
			myInput.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		else if(key == 2 && !isPressed)
			myInput.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		else if(key == 3 && isPressed)
			myInput.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
		else if(key == 3 && !isPressed)
			myInput.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
	}
	else{
	
		myInput.type = INPUT_KEYBOARD;
		myInput.ki.wVk = key;
		if(!isPressed)
			myInput.ki.dwFlags = KEYEVENTF_KEYUP;
		else
			myInput.ki.dwFlags = 0;

	
	}
	SendInput(1, &myInput, sizeof(myInput));
	return true;
}

bool CInputGrabberWin::InsertMouseWheel(int deltaX, int deltaY){
	return false; //TODO
}
#endif