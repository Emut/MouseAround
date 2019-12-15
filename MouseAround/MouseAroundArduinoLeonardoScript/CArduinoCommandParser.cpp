#define BUTTON_PRESS 0xC1
#define BUTTON_RELEASE 0xC2
#define MOUSE_MOVE 0xC3
#define BUTTON_RLS_ALL 0xC4
#define MOUSE_SCROLL 0xC5

#define LM_BUTTON 0x1
#define RM_BUTTON 0x2
#define MM_BUTTON 0x4

/*
FORMAT: HEADER(1B) - MESSAGE_ID(1B) - PAYLOAD(2B) - CHECKSUM(1B) 

*/

class CArduinoCommandParser {
	int bufferSize;
	void (*funcMouseMove)(char, char, char);
	void (*funcKeyAction)(char, bool);
	void (*funcReleaseAll)();

	int nLastSeenID;
	unsigned char* ucpBuffer;
	int nReadIndex;
	int nWriteIndex;
	int nBufferSize;
	int nBufferCount;

	unsigned char ReadFromBuffer(int par_nIndex) {
		int nTempReadIndex = nReadIndex + par_nIndex;
		if (nTempReadIndex >= nBufferSize)
			nTempReadIndex -= nBufferSize;
		return ucpBuffer[nTempReadIndex];
	}
	void DiscardFromBuffer(int par_nCount) {
		nReadIndex += par_nCount;
		if (nReadIndex >= nBufferSize)
			nReadIndex -= nBufferSize;

		nBufferCount -= par_nCount;
	}
public:

	CArduinoCommandParser(int par_nBufferSize) {
		bufferSize = par_nBufferSize;
		funcKeyAction = 0;
		funcMouseMove = 0;
		funcReleaseAll = 0;

		nLastSeenID = -1;
		ucpBuffer = new unsigned char[par_nBufferSize];
		nBufferSize = par_nBufferSize;
		nReadIndex = 0;
		nWriteIndex = 0;
		nBufferCount = 0;
	}

	void setMouseMoveCallback(void (*par_funcMouseMove)(char, char, char)) {
		funcMouseMove = par_funcMouseMove;
	}

	void setKeyActionCallback(void (*par_funcKeyAction)(char, bool)) {
		funcKeyAction = par_funcKeyAction;
	}

	void setReleaseAllCallback(void (*par_funcReleaseAll)()) {
		funcReleaseAll = par_funcReleaseAll;
	}

	void InsertInput(unsigned char par_ucInput) {
		ucpBuffer[nWriteIndex] = par_ucInput;
		++nWriteIndex;
		if (nWriteIndex >= nBufferSize)
			nWriteIndex = 0;
		++nBufferCount;

		while (nBufferCount >= 5) { //msg are 5B
			if (ReadFromBuffer(1) == nLastSeenID) {	//a retransmission
				DiscardFromBuffer(1);	//move 1 ahead
				continue;
			}
			unsigned char ucMessageType = ReadFromBuffer(0);
			unsigned char ucMessageID = ReadFromBuffer(1);
			unsigned char ucPayload1 = ReadFromBuffer(2);
			unsigned char ucPayload2 = ReadFromBuffer(3);
			unsigned char ucCheckSum = ReadFromBuffer(4);
			unsigned char ucCalculatedCheckSum = ucMessageType + ucMessageID + ucPayload1 + ucPayload2;
			if (ucCalculatedCheckSum != ucCheckSum) {
				DiscardFromBuffer(1);	//Invalid checksum
				continue;
			}

			if (ucMessageType < BUTTON_PRESS || ucMessageType > MOUSE_SCROLL) {
				DiscardFromBuffer(1);	//Invalid message type
				continue;
			}

			nLastSeenID = ucMessageID;

			if (ucMessageType == BUTTON_PRESS) {
				funcKeyAction(char(ucPayload1), true);
			}
			else if (ucMessageType == BUTTON_RELEASE) {
				funcKeyAction(char(ucPayload1), false);
			}
			else if (ucMessageType == MOUSE_MOVE) {
				funcMouseMove(char(ucPayload1), char(ucPayload2), 0);
			}
			else if (ucMessageType == MOUSE_SCROLL) {
				funcMouseMove(0, 0, char(ucPayload1));
			}
			else if (ucMessageType == BUTTON_RLS_ALL) {
				funcReleaseAll();
			}

			DiscardFromBuffer(5);
		}
	}
};