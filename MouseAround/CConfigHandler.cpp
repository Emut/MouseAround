#include "CConfigHandler.h"
#include <stdio.h>
#include <string.h>


CConfigHandler::CConfigHandler() {
	nArduinoComPort = -1;
}

CConfigHandler* CConfigHandler::getInstance() {
	static CConfigHandler * singleInstance = new CConfigHandler;
	return singleInstance;
}

bool CConfigHandler::ImportConfig(const char* par_cpFileName) {
	FILE* fpConfig = 0;
	fopen_s(&fpConfig, par_cpFileName, "rb");
	if (fpConfig == NULL) {
		printf("Error opening %s\n", par_cpFileName);
		return false;
	}
	fseek(fpConfig, 0, SEEK_END);
	int nFileSize = ftell(fpConfig);
	fseek(fpConfig, 0, SEEK_SET);

	char* cpFileBuffer = new char[nFileSize];
	fread(cpFileBuffer, 1, nFileSize, fpConfig);

	char* cpTemp = strstr(cpFileBuffer, "ARDU_COM = ");
	if(cpTemp)
		sscanf_s(cpTemp, "ARDU_COM = %d", &nArduinoComPort);

	PrintConfig();
	delete[] cpFileBuffer;
	return true;
}
bool CConfigHandler::ExportConfig(const char* par_cpFileName) {
	return false;
}

void CConfigHandler::PrintConfig() {
	printf("ARDU_COM = %d\n", nArduinoComPort);
}