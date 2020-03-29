#include "CConfigHandler.h"
#include <stdio.h>
#include <string.h>
#include <memory>
#include <iostream>


CConfigHandler* CConfigHandler::getInstance() {
	static CConfigHandler * singleInstance = new CConfigHandler;
	return singleInstance;
}

bool CConfigHandler::ImportConfig(const char* par_cpFileName) {
	borderlessEnableCmd = NULL;
	borderlessDisableCmd = NULL;

	FILE* fpConfig = 0;
	fpConfig = fopen(par_cpFileName, "rb");
	if (fpConfig == NULL) {
		printf("Config file %s does not exist\n", par_cpFileName);
		return false;
	}
	fseek(fpConfig, 0, SEEK_END);
	int nFileSize = ftell(fpConfig);
	fseek(fpConfig, 0, SEEK_SET);

	//there are many return paths, best is to use a smart ptr.
	std::shared_ptr<char> cpFileBuffer(new char[nFileSize]);	
	//char* cpFileBuffer = new char[nFileSize];
	fread(cpFileBuffer.get(), 1, nFileSize, fpConfig);
	fclose(fpConfig);

	int deviceCount = 0;
	char* cpTemp = strstr(cpFileBuffer.get(), "DEVICE_COUNT");
	if(cpTemp)
		sscanf(cpTemp, "DEVICE_COUNT=%d", &deviceCount);
	else{
		printf("Corrupted config!");
		return false;
	}

	for(int i = 0; i < deviceCount; ++i){
		char key[40];
		sprintf(key, "\nDEVICE_ID=%d", i);
		char* cpTemp = strstr(cpFileBuffer.get(), key);
		if(cpTemp){
			++cpTemp;	//move beyond the newline.
			HandlerProperties toRegister;
			int res = sscanf(cpTemp, "DEVICE_ID=%d DEVICE_NAME=%s PORT=%d IP_ADDR=%s WIDTH=%d HEIGTH=%d;", &toRegister.DEVICE_ID, toRegister.DEVICE_NAME, &toRegister.PORT,
					toRegister.IP_ADDR, &toRegister.width, &toRegister.heigth);
			devices.push_back(toRegister);
		}
		else{
			printf("Corrupted config!");
			return false;
		}
	}

	cpTemp = strstr(cpFileBuffer.get(), "HOTKEY_COUNT");
	int hotkeyCount = 0;
	if(cpTemp)
		sscanf(cpTemp, "HOTKEY_COUNT=%d", &hotkeyCount);
	else{
		printf("Corrupted config!");
		return false;
	}
	

	for(int i = 0; i < hotkeyCount; ++i){
		char key[40];
		sprintf(key, "\nHKEY_ID=%d", i);
		char* cpTemp = strstr(cpFileBuffer.get(), key);
		if(cpTemp){
			++cpTemp;
			int id = 0;
			int type = 0;
			const unsigned int keyMaxLen = 40;
			const unsigned int argsMaxLen = 200;
			char keys[keyMaxLen];
			char args[argsMaxLen];
			int res = sscanf(cpTemp, "HKEY_ID=%d KEYS=%s HKEY_TYPE=%d ARG=%[^\r\n];", &id, keys, &type, args);
			
			HotkeyInfo toRegister;
			toRegister.nHotkeyID = id;

			switch (type)
			{
			case 1:
				toRegister.setTypeAndArgs(SET_ACTIVE_SCREEN, args);
				break;
			case 2:
				toRegister.setTypeAndArgs(TOGGLE_BORDERLESS_MODE, args);
				break;
			case 3:
				if(args[strlen(args) - 1] == ';')
					args[strlen(args) - 1] = 0;		//omit the endofline ;
				toRegister.setTypeAndArgs(EXECUTE_CMD, args);
				break;
			default:
				break;
			}

			cpTemp = keys;
			int commaCount = 0;
			while(*cpTemp != 0){
				if(*cpTemp == ','){++commaCount;}
				++cpTemp;
			}

			toRegister.ucHotkeyLength = commaCount + 1;
			toRegister.ucpHotkeySequence = new unsigned char[toRegister.ucHotkeyLength];
			cpTemp = keys;
			for(int i = 0; i < toRegister.ucHotkeyLength; ++i){
				toRegister.ucpHotkeySequence[i] = (unsigned char)strtol(cpTemp, &cpTemp, 0);
				++cpTemp;
			}

			hotkeys.push_back(toRegister);
		}
		else{
			printf("Corrupted config!");
			return false;
		}

		cpTemp = strstr(cpFileBuffer.get(), "BORDERLESS_SETTINGS_COUNT");
		int blessSettingCount = 0;
		if(cpTemp)
			sscanf(cpTemp, "BORDERLESS_SETTINGS_COUNT=%d", &blessSettingCount);
		else{
			printf("Corrupted config!");
			return false;
		}

		for(int i = 0; i < blessSettingCount; ++i){
			char key[40];
			sprintf(key, "\nBLESS_SET_ID=%d", i);
			char* cpTemp = strstr(cpFileBuffer.get(), key);
			if(cpTemp){
				++cpTemp;	//move beyond the newline.
				BorderlessInfo temp;
				int settingId;
				int deviceId;
				int res = sscanf(cpTemp, "BLESS_SET_ID=%d DEVICE_ID=%d LEFT=%d,%d RIGHT=%d,%d UP=%d,%d DOWN=%d,%d;", 
					&settingId, &deviceId, &temp.leftBorder, &temp.leftNeigh, &temp.rightBorder, &temp.rightNeigh,
						&temp.topBorder, &temp.topNeigh, &temp.bottomBorder, &temp.bottomNeigh);
				devices[deviceId].itsBorders = temp;
			}
			else{
				printf("Corrupted config!");
				return false;
			}
		}

		cpTemp = strstr(cpFileBuffer.get(), "\nBLESS_ENABLE_CMD");
		if(cpTemp != NULL){
			const unsigned int argsMaxLen = 200;
			char args[argsMaxLen];
			sscanf(cpTemp+1, "BLESS_ENABLE_CMD=%[^\r\n]", args);
			if(args[strlen(args) - 1] == ';')
					args[strlen(args) - 1] = 0;		//omit the endofline ;
			borderlessEnableCmd = new char[strlen(args) + 1];
			strcpy(borderlessEnableCmd, args);
		}

		cpTemp = strstr(cpFileBuffer.get(), "\nBLESS_DISABLE_CMD");
		if(cpTemp != NULL){
			const unsigned int argsMaxLen = 200;
			char args[argsMaxLen];
			sscanf(cpTemp+1, "BLESS_DISABLE_CMD=%[^\r\n]", args);
			if(args[strlen(args) - 1] == ';')
					args[strlen(args) - 1] = 0;		//omit the endofline ;
			borderlessDisableCmd = new char[strlen(args) + 1];
			strcpy(borderlessDisableCmd, args);
		}
	}


	PrintConfig();
	return true;
}
bool CConfigHandler::ExportConfig(const char* par_cpFileName) {
	FILE* filep = 0;
	filep = fopen(par_cpFileName, "wb");
	if(filep == NULL){
		printf("Error: Can not create config file\n");
		return false;
	}
	printf("Creating file %s\n", par_cpFileName);
	fprintf(filep, "#MouseAroundConfigFile\r\n");
	fprintf(filep, "#Sample Device Registirations:\r\n");
	//TODO: add lines

	fclose(filep);
	return true;
}

void CConfigHandler::PrintConfig() {

	for(const auto it: devices){
		it.Print();
	}
	printf("BLESS_ENABLE_CMD:%s\n", borderlessEnableCmd);
	printf("BLESS_ENABLE_CMD:%s\n", borderlessDisableCmd);
	printf("HOTKEY_COUNT = %d\n", (int)hotkeys.size());
	for(const auto& it : hotkeys){
		it.Print();
	}
}