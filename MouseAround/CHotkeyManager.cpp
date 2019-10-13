#include "CHotkeyManager.h"
#include <string>
#include <stdio.h>

CHotkeyManager::CHotkeyManager() {
	printf("ChotkeyManager::Constructor\n");
	nHotkeyCount = 0;

	for (int nInd = 0; nInd < 256; ++nInd)
		bpKeyStatus[nInd] = false;
}

int CHotkeyManager::KeyUpdate(unsigned char ucKey, bool bIsPressed) {
	bpKeyStatus[ucKey] = bIsPressed;
	if (!bIsPressed)
		return -1;	//If a key release, can not trigger a hotkey
	std::list<tsHotkeyInfo*>::iterator it;
	for (it = listHotkeys.begin(); it != listHotkeys.end(); ++it) {
		bool bComboFits = true;
		for (int nInd = 0; nInd < (*it)->ucHotkeyLength; ++nInd) {
			if (!bpKeyStatus[(*it)->ucpHotkeySequence[nInd]]) {
				bComboFits = false;
				break;
			}
		}
		if (bComboFits)
			return (*it)->nHotkeyID;
	}
	return -1;
}
int CHotkeyManager::RegisterKeyCombination(unsigned char* ucpKeys, int nKeyCount, int nHotkeyID) {
	
	tsHotkeyInfo* stpTemp = new tsHotkeyInfo;
	stpTemp->nHotkeyID = nHotkeyID;
	stpTemp->ucHotkeyLength = nKeyCount;
	stpTemp->ucpHotkeySequence = new unsigned char[nKeyCount];

	for (int nInd = 0; nInd < nKeyCount; ++nInd)
		stpTemp->ucpHotkeySequence[nInd] = ucpKeys[nInd];

	listHotkeys.push_back(stpTemp);
	++nHotkeyCount;

	return nHotkeyID;
}