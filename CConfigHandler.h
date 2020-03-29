#pragma once
#include <vector>
#include "DataTypes.h"

class CConfigHandler
{
	
public:
	std::vector<HandlerProperties> devices;
	std::vector<HotkeyInfo> hotkeys;
	char* borderlessEnableCmd;
	char* borderlessDisableCmd;

	static CConfigHandler* getInstance();
	bool ImportConfig(const char* par_cpFileName);
	bool ExportConfig(const char* par_cpFileName);
	void PrintConfig();
};

