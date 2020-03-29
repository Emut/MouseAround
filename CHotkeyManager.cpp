#include "CHotkeyManager.h"
#include <string>
#include <stdio.h>

CHotkeyManager::CHotkeyManager() {
	//printf("ChotkeyManager::Constructor\n");
	itsHotkeys = &CConfigHandler::getInstance()->hotkeys;
	dummyHK.type = DUMMY;

	for (int nInd = 0; nInd < 256; ++nInd)
		bpKeyStatus[nInd] = false;
}

const HotkeyInfo& CHotkeyManager::KeyUpdate(unsigned char ucKey, bool bIsPressed) {
	bpKeyStatus[ucKey] = bIsPressed;
	if (!bIsPressed)
		return dummyHK;	//If a key release, can not trigger a hotkey
	std::vector<HotkeyInfo>::iterator it;
	for (const auto& it : *itsHotkeys) {
		bool bComboFits = true;
		for (int nInd = 0; nInd < it.ucHotkeyLength; ++nInd) {
			if (!bpKeyStatus[it.ucpHotkeySequence[nInd]]) {
				bComboFits = false;
				break;
			}
		}
		if (bComboFits){
			for (int nInd = 0; nInd < it.ucHotkeyLength; ++nInd) {
				bpKeyStatus[it.ucpHotkeySequence[nInd]] = false; //consume the used hotkey presses
				//otherwise CTRL+ALT+3 ... 3 .... 3 combo would yield 3 hotkey matches. 
			}
			return it;
		}
	}
	return dummyHK;
}
