#pragma once
#include "CInputGrabberWin.h"
#include "CHotkeyManager.h"
#include "IScreenHandler.h"
#include <vector>

class CManager: public IInputGrabberOut
{
	int nOwnScreenWidth;
	int nOwnScreenHeigth;
	int nVirtualScreenPosX;
	int nVirtualScreenPosY;
	bool bOwnScreenActive;
	bool IsInOwnScreen(int nX, int nY);
	int nActiveScreenID;
	int nScreenCount;
	bool bBorderlessModeActive;
	std::vector<IScreenHandlerIn*> vectScreenp;

	enum teHotkeys
	{
		HOTKEY_BORDERLESS_MODE,
		HOTKEY_SET_ACTIVE_SCREEN_BEGIN
	};

	void HandleHotkey(teHotkeys eHotkey);
public:
	CManager(const char* cpExecName, int nOwnScreenWidth, int nOwnScreenHeigth);
	~CManager();
	bool MouseUpdated(int, int, int, int);
	bool KeyboardUpdated(unsigned char, bool);

	IInputGrabberIn* itsInputGrabber;
	CHotkeyManager itsHotkeyManager;
};

