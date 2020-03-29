#include "IScreenHandler.h"
#include "CSem.h"
#include "CInputGrabberFactory.h"
#include <thread>
#include "CSocket.h"
#include <atomic>

class CSelfHandler: public IScreenHandlerIn{
public:
	void SendRelativeMouseInput(int nDelX, int nDelY, bool bIsWheel);
	void SendExactMouseInput(int nX, int nY);
	void SendKeyboardInput(unsigned char ucKey, bool bIsPressed);
	void getMousePos(int& nX, int& nY);

	CSelfHandler(unsigned short port);

private:
	void ActiveFunction();
	void SocketListener();
	CSem sem;
	IInputGrabberIn* itsInputGrabber;
	std::thread* threadCaller;
	std::thread* threadSocketListener;
	int nExactPosInputX;
	int nExactPosInputY;
	int m_wheelDeltaX;
	int m_wheelDeltaY;
	unsigned char key;
	bool isPressed;
	std::atomic<bool> mouseCall;
	std::atomic<bool> keyCall;
	std::atomic<bool> wheelCall;
	CSocket* itsSocket;
	
};