#ifndef CSOCKET_H
#define CSOCKET_H

#ifdef _WIN32
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
#endif
#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

class CSocket{

public:
	CSocket(unsigned short destPort, const char* destIP, unsigned short srcPort, const char* srcIP);
	int Send(void* msg, int count);
	int Read(void* buf, int maxCount);


private:
	sockaddr_in to_addr;
	sockaddr_in to_me;
	int nSocket;
};


#endif