/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "NetClientUDP.hpp"
#include "BigError.hpp"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#endif

using namespace GameEngine;

void NetClientUDP::Initialize(uint16 port, strgv ip)
{
	mSocket = INVALID_SOCKET;
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	NetClient::Initialize(port, ip);
	mServiceLen = sizeof(mService);
}

int NetClientUDP::Run()
{
	return ListenToServer();
}

int NetClientUDP::Recv(char* buffer, int size, int flags)
{
	return recvfrom(mSocket, buffer, size, flags, (SOCKADDR*)&mService, &mServiceLen);
}

int NetClientUDP::Send(const char* buffer, int size, int flags)
{
	return sendto(mSocket, buffer, size, flags, (SOCKADDR*)&mService, mServiceLen);
}
