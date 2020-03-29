#pragma once
#include "CConfigHandler.h"
#include <list>


class CHotkeyManager
{		


	std::vector<HotkeyInfo>* itsHotkeys;
	bool bpKeyStatus[256];
	HotkeyInfo dummyHK;
public:
	CHotkeyManager();
	const HotkeyInfo& KeyUpdate(unsigned char ucKey, bool bIsPressed);
	//int RegisterKeyCombination(unsigned char* ucpKeys, int nKeyCount, int nKeyID);
};

