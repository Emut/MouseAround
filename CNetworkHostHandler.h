#include "IScreenHandler.h"
#include "CSocket.h"

class CNetworkHostHandler: public IScreenHandlerIn{
public:
	CNetworkHostHandler(int width, int heigth, const char* destAddress, unsigned short destPort, const char* sourceAddress = 0, unsigned short sourcePort = 0);

	void SendRelativeMouseInput(int nDelX, int nDelY, bool bIsWheel);
	void SendExactMouseInput(int nX, int nY);
	void SendKeyboardInput(unsigned char ucKey, bool bIsPressed);

	void getMousePos(int& nX, int& nY);

private:
	CSocket* itsSocket;
	int xPos;
	int yPos;
	int m_width;
	int m_heigth;
};