#include "CArduinoHandler.h"
#include "KeyboardTypeHelpers.h"



CArduinoHandler::CArduinoHandler(int nComID) {


	ucMessageID = 0;

	LPWSTR b = new WCHAR[20];
	b[0] = '\\';
	b[1] = '\\';
	b[2] = '.';
	b[3] = '\\';
	b[4] = 'C';
	b[5] = 'O';
	b[6] = 'M';
	if (nComID > 9) {
		b[7] = nComID / 10 + '0';
		b[8] = nComID % 10 + '0';
		b[9] = '\0';
	}
	else {
		b[7] = nComID % 10 + '0';
		b[8] = '\0';
	}
	hndComm = CreateFile(b,                //port name
		GENERIC_READ | GENERIC_WRITE, //Read/Write
		0,                            // No Sharing
		NULL,                         // No Security
		OPEN_EXISTING,// Open existing port only
		0,            // Non Overlapped I/O
		NULL);        // Null for Comm Devices
	delete[] b;
	if (hndComm == INVALID_HANDLE_VALUE)
		printf("Error in opening serial port\n");
	else
		printf("opening serial port successful\n");

	DCB dcbSerialParams = { 0 }; // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	bool Status = false;
	Status = GetCommState(hndComm, &dcbSerialParams);

	dcbSerialParams.BaudRate = CBR_115200;  // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;  // Setting Parity = None


	SetCommState(hndComm, &dcbSerialParams);

	char lpBuffer[] = "A";
	DWORD dNoOFBytestoWrite;         // No of bytes to write into the port
	DWORD dNoOfBytesWritten = 0;     // No of bytes written to the port
	dNoOFBytestoWrite = sizeof(lpBuffer);
}

void CArduinoHandler::SendRelativeMouseInput(int nDelX, int nDelY, bool bIsWheel) {
	mtxWriteLock.lock();
	char cpBuffer[5];
	if (!bIsWheel) {
		printf("%d,%d...\n", nDelX, nDelY);
		if (nDelX > 127)
			nDelX = 127;
		if (nDelY > 127)
			nDelY = 127;
		if (nDelX < -127)
			nDelX = -127;
		if (nDelY < -127)
			nDelY = -127;
		
		cpBuffer[0] = 0xC3;
		cpBuffer[1] = (char)ucMessageID;
		cpBuffer[2] = (char)nDelX;
		cpBuffer[3] = (char)nDelY;
		unsigned char ucCheckSum = 0;
		ucCheckSum += cpBuffer[0];
		ucCheckSum += cpBuffer[1];
		ucCheckSum += cpBuffer[2];
		ucCheckSum += cpBuffer[3];
		cpBuffer[4] = ucCheckSum;
	}
	else {
		cpBuffer[0] = 0xC5;
		cpBuffer[1] = (char)ucMessageID;
		cpBuffer[2] = (char)(nDelX / 120);
		cpBuffer[3] = 0;
		unsigned char ucCheckSum = 0;
		ucCheckSum += cpBuffer[0];
		ucCheckSum += cpBuffer[1];
		ucCheckSum += cpBuffer[2];
		ucCheckSum += cpBuffer[3];
		cpBuffer[4] = ucCheckSum;
	}
	bool bStatus = false;
	DWORD dNoOfBytesWritten = 0;
	bStatus = WriteFile(hndComm, cpBuffer, 5, &dNoOfBytesWritten, NULL);
	bStatus = WriteFile(hndComm, cpBuffer, 5, &dNoOfBytesWritten, NULL);
	++ucMessageID;
	mtxWriteLock.unlock();
}
void CArduinoHandler::SendExactMouseInput(int nX, int nY) {

}
void CArduinoHandler::SendKeyboardInput(unsigned char ucKey, bool bIsPressed) {
	mtxWriteLock.lock();
	unsigned char ucTemp = ucKey;
	if (!KeyboardTypeHelper::WinToArduino(ucKey)) {
		mtxWriteLock.unlock();
		return;			//input key is not applicable for arduino
	}

	char cpBuffer[5];
	if (bIsPressed)
		cpBuffer[0] = 0xC1;
	else
		cpBuffer[0] = 0xC2;
	cpBuffer[1] = (char)ucMessageID;
	cpBuffer[2] = ucKey;
	cpBuffer[3] = 0;
	unsigned char ucCheckSum = 0;
	ucCheckSum += cpBuffer[0];
	ucCheckSum += cpBuffer[1];
	ucCheckSum += cpBuffer[2];
	ucCheckSum += cpBuffer[3];
	cpBuffer[4] = ucCheckSum;

	bool bStatus = false;
	DWORD dNoOfBytesWritten = 0;
	bStatus = WriteFile(hndComm, cpBuffer, 5, &dNoOfBytesWritten, NULL);
	bStatus = WriteFile(hndComm, cpBuffer, 5, &dNoOfBytesWritten, NULL);

	++ucMessageID;
	mtxWriteLock.unlock();
}

CArduinoHandler::~CArduinoHandler() {
	CloseHandle(hndComm);	//Closing the Serial Port
}
