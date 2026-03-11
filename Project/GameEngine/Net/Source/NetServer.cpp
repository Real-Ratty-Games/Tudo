/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "NetServer.hpp"
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

void NetServer::Initialize(uint16 port, strgv ip)
{
	mPort	= port;
	mIP		= ip;

	if (mSocket == INVALID_SOCKET)
#if _WIN32
		throw BigError("Failed creating server socket: " + std::to_string(WSAGetLastError()));
#else
		throw BigError("Failed creating server socket: " + std::to_string(errno));
#endif

	ulong nb = 1;
	ioctlsocket(mSocket, FIONBIO, &nb); // set non-blocking

	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, ip.data(), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(mSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
#if _WIN32
		throw BigError("Failed binding server: " + std::to_string(WSAGetLastError()));
#else
		throw BigError("Failed binding server: " + std::to_string(errno));
#endif
}
