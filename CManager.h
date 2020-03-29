#pragma once
#include "CInputGrabberWin.h"
#include "CHotkeyManager.h"
#include "IScreenHandler.h"
#include "CConfigHandler.h"
#include "IInputGrabber.h"
#include "CSem.h"
#include <vector>

class CManager: public IInputGrabberOut
{
	unsigned int nActiveScreenID;
	bool bBorderlessModeActive;
	std::vector<IScreenHandlerIn*> vectScreenp;

	void HandleHotkey(const HotkeyInfo& hk);
public:
	CManager(const char* cpExecName);
	~CManager();
	bool MouseUpdated(int, int, int, int, bool);
	bool KeyboardUpdated(unsigned char, bool);

	IInputGrabberIn* itsInputGrabber;
	CHotkeyManager itsHotkeyManager;

private:
	class CSysCmdExecuter
	{
	public:
		CSysCmdExecuter();
		~CSysCmdExecuter();
		bool ExecuteCmd(const char* par_cpCmd);
	private:
		CSem itsSem;
		void ActiveFunction(CSem* itsSem);
		const char* command;
	};

	CSysCmdExecuter itsCmdExecuter;
};

