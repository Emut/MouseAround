#pragma once
#include <string>
#include <list>


class CHotkeyManager
{		
	struct tsHotkeyInfo
	{
		unsigned char* ucpHotkeySequence;
		unsigned char ucHotkeyLength;
		int nHotkeyID;
	};

	std::list<tsHotkeyInfo*> listHotkeys;
	int nHotkeyCount;

	bool bpKeyStatus[256];
public:
	CHotkeyManager();
	int KeyUpdate(unsigned char ucKey, bool bIsPressed);
	int RegisterKeyCombination(unsigned char* ucpKeys, int nKeyCount, int nKeyID);
};

