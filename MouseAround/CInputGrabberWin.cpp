#include "CInputGrabberWin.h"
#include <stdio.h>
#include <windows.h>
#define _WIN32_WINNT 0x0400
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
	if (!hInstance) hInstance = LoadLibrary((LPCWSTR)lpParam);
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
	if (!hInstance) hInstance = LoadLibrary((LPCWSTR)lpParam);
	if (!hInstance) return 1;

	callbackInstance->hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEvent, hInstance, NULL);
	MessageLoop();
	UnhookWindowsHookEx(callbackInstance->hKeyboardHook);
	return 0;
}

CInputGrabberWin::CInputGrabberWin(const char* cpExecName, IInputGrabberOut* par_itsOwner) {
	printf("CInputGrabberWin::Constructor\n");
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
	if (pMouseStruct != NULL && wParam == WM_MOUSEMOVE) {
		int delX = pMouseStruct->pt.x - prevMousePos.x;
		int delY = pMouseStruct->pt.y - prevMousePos.y;
		
		if (itsOwner->MouseUpdated(pMouseStruct->pt.x, pMouseStruct->pt.y, delX, delY))
			return -1;
		else{
			prevMousePos = pMouseStruct->pt;
			return 0;
		}
	}
	return 0;
}

LRESULT CInputGrabberWin::KeyboardCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT* pKbStruct = (KBDLLHOOKSTRUCT*)lParam;
	if(itsOwner->KeyboardUpdated(pKbStruct->vkCode, wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
		return -1;
	return 0;
}

bool CInputGrabberWin::setMousePos(int nPosX, int nPosY) {
	prevMousePos.x = nPosX;
	prevMousePos.y = nPosY;
	return SetCursorPos(nPosX, nPosY);
}

bool CInputGrabberWin::getMousePos(int* nPosX, int* nPosY) {
	POINT ptTemp;
	GetCursorPos(&ptTemp);
	*nPosX = ptTemp.x;
	*nPosY = ptTemp.y;
	return true;
}