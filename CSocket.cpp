#include "CSocket.h"
#include <iostream>

CSocket::CSocket(unsigned short destPort, const char* destIP, unsigned short srcPort, const char* srcIP){
#if _WIN32
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
	    printf("WSAStartup failed.\n");
	    return;
	}
#endif
	to_addr.sin_family      = AF_INET;
	to_addr.sin_port        = htons(destPort);          
	to_addr.sin_addr.s_addr = (destIP == NULL) ? 0 : inet_addr(destIP);      
	
	to_me.sin_family        = AF_INET;
	to_me.sin_port          = htons(srcPort);
	to_me.sin_addr.s_addr   = (srcIP == NULL) ? 0 : inet_addr(srcIP);
	
	
	if ((nSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("talker: socket");
	}

	if ( bind( nSocket, (sockaddr*)&to_me, sizeof( to_me ) ) == -1)
	{
		printf("Error binding/n");
		return;
	}
}


int CSocket::Send(void* msg, int count){
	auto nSockAdrSize = sizeof(sockaddr_in);
	auto nSendCount =  sendto(nSocket, (char*)msg, count, 0, (struct sockaddr*)&to_addr, nSockAdrSize);
	return nSendCount;
}

int CSocket::Read(void* buf, int maxCount){
	
	int nTempSockAdrSize = sizeof(sockaddr_in);                  
	auto nReadCount = recv(nSocket, (char*)buf, maxCount, 0);
	return nReadCount;
}