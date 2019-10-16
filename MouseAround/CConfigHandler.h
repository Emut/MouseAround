#pragma once
class CConfigHandler
{
	
	CConfigHandler();

public:
	int nArduinoComPort;

	static CConfigHandler* getInstance();
	bool ImportConfig(const char* par_cpFileName);
	bool ExportConfig(const char* par_cpFileName);
	void PrintConfig();
};

