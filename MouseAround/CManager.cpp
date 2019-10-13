#include "CManager.h"
#include <stdio.h>
#include "CArduinoHandler.h"

CManager::CManager(const char* cpExecName, int par_nOwnScreenWidth, int par_nOwnScreenHeigth) {
	bOwnScreenActive = true;
	nOwnScreenWidth = par_nOwnScreenWidth;
	nOwnScreenHeigth = par_nOwnScreenHeigth;
	
	itsInputGrabber = new CInputGrabberWin(cpExecName, this);
	itsInputGrabber->getMousePos(&nVirtualScreenPosX, &nVirtualScreenPosY);
	nScreenCount = 1;
	nActiveScreenID = 0;
	bBorderlessModeActive = false;

	unsigned char ucpTemp[10];
	ucpTemp[0] = 49;
	ucpTemp[1] = 162;
	ucpTemp[2] = 164;
	itsHotkeyManager.RegisterKeyCombination(ucpTemp, 3, HOTKEY_SET_ACTIVE_SCREEN_BEGIN);
	ucpTemp[0] = 50;
	itsHotkeyManager.RegisterKeyCombination(ucpTemp, 3, HOTKEY_SET_ACTIVE_SCREEN_BEGIN + 1);

	new CArduinoHandler(19);
}

CManager::~CManager() {

}

bool CManager::MouseUpdated(int posX, int posY, int delX, int delY) {
	nVirtualScreenPosX += delX;
	nVirtualScreenPosY += delY;
	bool bIsVirtualMouseInOwnScreen = IsInOwnScreen(nVirtualScreenPosX, nVirtualScreenPosY);
	bool bOverrideInputOnSelf = false;

	if(bIsVirtualMouseInOwnScreen && bOwnScreenActive){}

	if (posX > nOwnScreenWidth) {
		posX = nOwnScreenWidth;
		bOverrideInputOnSelf = true;
	}
	if (posY > nOwnScreenHeigth) {
		posY = nOwnScreenHeigth;
		bOverrideInputOnSelf = true;
	}
	//printf("vposx=%d vposy=%d posx=%d posy=%d delx=%d dely=%d\n", nVirtualScreenPosX, nVirtualScreenPosY,
		//posX, posY, delX, delY);
	if (bOverrideInputOnSelf) {
		printf("or");
		itsInputGrabber->setMousePos(posX, posY);
		return true;
	}
	return false;
}

bool CManager::KeyboardUpdated(unsigned char ucKey, bool bIsPressed) {
	//printf("Cman::KeybUpdated %d %s\n", ucKey, bIsPressed?"pressed":"released");
	int nHotKey = itsHotkeyManager.KeyUpdate(ucKey, bIsPressed);
	if (nHotKey != -1)
		printf("Hotkey %d detected\n", nHotKey);
	return false;
}

bool CManager::IsInOwnScreen(int nX, int nY) {
	if (nX < 0 || nX > nOwnScreenWidth)
		return false;
	if (nY < 0 || nY > nOwnScreenHeigth)
		return false;
	return true;
}

void CManager::HandleHotkey(teHotkeys eHotkey) {
	if (eHotkey >= HOTKEY_SET_ACTIVE_SCREEN_BEGIN) {
		nActiveScreenID = eHotkey - HOTKEY_SET_ACTIVE_SCREEN_BEGIN;
	}
}
