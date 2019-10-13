#include "CArduinoHandler.h"


CArduinoHandler::CArduinoHandler(int nComID) {
	char cpCommName[20];
	sprintf_s(cpCommName, "\\\\.\\COM%d", nComID);
	hndComm = CreateFile((LPCWSTR)cpCommName,   //port name
		GENERIC_READ | GENERIC_WRITE, //Read/Write
		0,                            // No Sharing
		NULL,                         // No Security
		OPEN_EXISTING,// Open existing port only
		0,            // Non Overlapped I/O
		NULL);        // Null for Comm Devices

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
	char cpBuffer[20];
	cpBuffer[0] = 0xC3;
	cpBuffer[1] = (short)nDelX >> 8;
	cpBuffer[2] = (short)nDelX & 0xFF;
	cpBuffer[3] = (short)nDelY >> 8;
	cpBuffer[4] = (short)nDelY & 0xFF;

	bool bStatus = false;
	DWORD dNoOfBytesWritten = 0;
	bStatus = WriteFile(hndComm,        // Handle to the Serial port
		cpBuffer,     // Data to be written to the port
		5,  //No of bytes to write
		&dNoOfBytesWritten, //Bytes written
		NULL);
}
void CArduinoHandler::SendExactMouseInput(int nX, int nY) {

}
void CArduinoHandler::SendKeyboardInput(unsigned char ucKey, bool bIsPressed) {

}

