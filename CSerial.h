#pragma once
#ifdef _WIN32
#include <Windows.h>
#endif

class CSerial{

public:
	CSerial(int port);
	int Send(void* msg, int count);
	int Read(void* buf, int maxCount);


private:
	int m_port;
	#ifdef _WIN32
	HANDLE hndComm;
	#endif
};

