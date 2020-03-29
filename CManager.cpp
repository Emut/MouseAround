#include "CManager.h"
#include <stdio.h>
#include "CArduinoHandler.h"
#include "CNetworkHostHandler.h"
#include "CSelfHandler.h"
#include <thread>
#include "CConfigHandler.h"
#include "CHandlerFactory.h"


CManager::CManager(const char* cpExecName) {

	if(!CConfigHandler::getInstance()->ImportConfig("MAr_config.ini"))	//if does not exist or is corrupt, create the default config
		CConfigHandler::getInstance()->ExportConfig("MAr_config_sample.ini");


	itsInputGrabber = CInputGrabberFactory::CreateInstance(cpExecName, this);
	nActiveScreenID = 0;
	bBorderlessModeActive = false;
	
	for(const auto& i: CConfigHandler::getInstance()->devices)
		vectScreenp.push_back(CHandlerFactory::CreateHandler(i));
}

CManager::~CManager() {
	for(auto i: vectScreenp)	//destruct the handlers
		delete i;
	delete itsInputGrabber;
}

bool CManager::MouseUpdated(int posX, int posY, int delX, int delY, bool bIsWheel) {
	if (bBorderlessModeActive && !bIsWheel) {	
		auto activeDevBorders = CConfigHandler::getInstance()->devices[nActiveScreenID].itsBorders;
		int activeX;
		int activeY;
		vectScreenp[nActiveScreenID]->getMousePos(activeX,activeY);
		//printf("%d,%d  %d,%d, %d\n", delX, delY, activeX, activeY, nActiveScreenID);
		if(delX > 0 && activeDevBorders.rightNeigh != -1){
			if(activeX + delX > activeDevBorders.rightBorder){
				//printf("Corrective: border:%d, activeX:%d %d,%d\n",activeDevBorders.rightBorder, activeX, activeDevBorders.rightBorder - activeX, delY);
				vectScreenp[nActiveScreenID]->SendRelativeMouseInput(activeDevBorders.rightBorder - activeX, delY, false);
				nActiveScreenID = activeDevBorders.rightNeigh;
				auto newActiveDevBorders = CConfigHandler::getInstance()->devices[nActiveScreenID].itsBorders;
				vectScreenp[nActiveScreenID]->SendExactMouseInput(newActiveDevBorders.leftBorder,activeY);
				return true;
			}
		}
		else if(delX < 0 && activeDevBorders.leftNeigh != -1){
			if(activeX + delX < activeDevBorders.leftBorder){
				vectScreenp[nActiveScreenID]->SendRelativeMouseInput(activeDevBorders.leftBorder - activeX, delY, false);
				nActiveScreenID = activeDevBorders.leftNeigh;
				auto newActiveDevBorders = CConfigHandler::getInstance()->devices[nActiveScreenID].itsBorders;
				vectScreenp[nActiveScreenID]->SendExactMouseInput(newActiveDevBorders.rightBorder,activeY);
				return true;
			}
		}

		if(delY > 0 && activeDevBorders.bottomNeigh != -1){
			if(activeY + delY > activeDevBorders.bottomBorder){
				vectScreenp[nActiveScreenID]->SendRelativeMouseInput(delX, activeDevBorders.bottomBorder - activeY, false);
				nActiveScreenID = activeDevBorders.bottomNeigh;
				auto newActiveDevBorders = CConfigHandler::getInstance()->devices[nActiveScreenID].itsBorders;
				vectScreenp[nActiveScreenID]->SendExactMouseInput(activeX,newActiveDevBorders.topBorder);
				return true;
			}
		}
		else if(delY < 0 && activeDevBorders.topNeigh != -1){
			if(activeY + delY < activeDevBorders.topBorder){
				vectScreenp[nActiveScreenID]->SendRelativeMouseInput(delX, activeDevBorders.topBorder - activeY, false);
				nActiveScreenID = activeDevBorders.topNeigh;
				auto newActiveDevBorders = CConfigHandler::getInstance()->devices[nActiveScreenID].itsBorders;
				vectScreenp[nActiveScreenID]->SendExactMouseInput(activeX,newActiveDevBorders.bottomBorder);
				return true;
			}
		}

	}
	
		if (nActiveScreenID == 0)
			return false;
		if (vectScreenp.size() >= nActiveScreenID && vectScreenp[nActiveScreenID] != NULL) {
			
			vectScreenp[nActiveScreenID]->SendRelativeMouseInput(delX, delY, bIsWheel);
			return true;
		}
		else
			return false;
	
	return false;
}


bool CManager::KeyboardUpdated(unsigned char ucKey, bool bIsPressed) {
	//printf("Cman::KeybUpdated %d 1%s\n", ucKey, bIsPressed?"pressed":"released");
	auto HotKey = itsHotkeyManager.KeyUpdate(ucKey, bIsPressed);
	if (HotKey.type != DUMMY) {
		printf("Hotkey %d detected\n", HotKey.nHotkeyID);
		HandleHotkey(HotKey);
	}
	if (nActiveScreenID == 0)
		return false;
	if (vectScreenp.size() >= nActiveScreenID && vectScreenp[nActiveScreenID] != NULL) {
		vectScreenp[nActiveScreenID]->SendKeyboardInput(ucKey, bIsPressed);
		return true;
	}
	else
		return false;
}


void CManager::HandleHotkey(const HotkeyInfo& hk) {
	switch (hk.type) {
	case TOGGLE_BORDERLESS_MODE:
		bBorderlessModeActive = !bBorderlessModeActive;
		if(bBorderlessModeActive){
			const char* cmd = CConfigHandler::getInstance()->borderlessEnableCmd;
			if(cmd != NULL)
				itsCmdExecuter.ExecuteCmd(cmd);
		}
		else{
			const char* cmd = CConfigHandler::getInstance()->borderlessDisableCmd;
			if(cmd != NULL)
				itsCmdExecuter.ExecuteCmd(cmd);
		}
		break;
	case SET_ACTIVE_SCREEN:
		nActiveScreenID = *(int*)hk.args;
		break;
	case EXECUTE_CMD:
		printf("executing:%s\n", (char*)hk.args);
		itsCmdExecuter.ExecuteCmd((char*)hk.args);
		break;
	default:
		break;
	}
}

CManager::CSysCmdExecuter::CSysCmdExecuter(){
	new std::thread(&CSysCmdExecuter::ActiveFunction, this, &itsSem);	//fire and forget
}

CManager::CSysCmdExecuter::~CSysCmdExecuter(){
	//TODO:delete the thread
}

bool CManager::CSysCmdExecuter::ExecuteCmd(const char* par_cpCmd){
	command = par_cpCmd;
	itsSem.Signal();
	return true;
}


void CManager::CSysCmdExecuter::ActiveFunction(CSem* itsSem){
	while(true){
		itsSem->Wait();
		system(command);
	}
}