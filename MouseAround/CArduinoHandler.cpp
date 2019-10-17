#include "CArduinoHandler.h"
#include "KeyboardTypeHelpers.h"


CArduinoHandler::CArduinoHandler(int nComID) {
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
	//printf("ArduRM %d, %d\n", nDelX, nDelY);
	char cpBuffer[5];
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
		&dNoOfBytesWritten, //Bytes written>
		NULL);
	
}
void CArduinoHandler::SendExactMouseInput(int nX, int nY) {

}
void CArduinoHandler::SendKeyboardInput(unsigned char ucKey, bool bIsPressed) {
	unsigned char ucTemp = ucKey;
	if (!KeyboardTypeHelper::WinToArduino(ucKey))
		return;		//input key is not applicable for arduino
	char cpBuffer[2];
	if (bIsPressed)
		cpBuffer[0] = 0xC1;
	else
		cpBuffer[0] = 0xC2;

	cpBuffer[1] = ucKey;

	bool bStatus = false;
	DWORD dNoOfBytesWritten = 0;
	bStatus = WriteFile(hndComm,        // Handle to the Serial port
		cpBuffer,     // Data to be written to the port
		2,  //No of bytes to write
		&dNoOfBytesWritten, //Bytes written>
		NULL);
}

CArduinoHandler::~CArduinoHandler() {
	CloseHandle(hndComm);	//Closing the Serial Port
}
