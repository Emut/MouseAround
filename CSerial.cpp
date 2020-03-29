#include "CSerial.h"
#include <stdio.h>

CSerial::CSerial(int port){
	m_port = port;
	#ifdef _WIN32
	LPSTR b = new CHAR[10];
	b[0] = '\\';
	b[1] = '\\';
	b[2] = '.';
	b[3] = '\\';
	b[4] = 'C';
	b[5] = 'O';
	b[6] = 'M';
	if (m_port > 9) {
		b[7] = m_port / 10 + '0';
		b[8] = m_port % 10 + '0';
		b[9] = '\0';
	}
	else {
		b[7] = m_port % 10 + '0';
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
		printf("Error in opening serial port for ArduinoHandler\n");

	DCB dcbSerialParams = { 0 }; // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	GetCommState(hndComm, &dcbSerialParams);

	dcbSerialParams.BaudRate = CBR_115200;  // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;  // Setting Parity = None


	SetCommState(hndComm, &dcbSerialParams);
	#endif
}

int CSerial::Send(void* msg, int count){
	#ifdef _WIN32
	DWORD dNoOfBytesWritten = 0;
	WriteFile(hndComm, msg, count, &dNoOfBytesWritten, NULL);
	return dNoOfBytesWritten;
	#endif
	return -1;
}
int CSerial::Read(void* buf, int maxCount){
	//not needed yet
	return -1;
}