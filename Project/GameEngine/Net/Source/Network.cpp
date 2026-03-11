/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "Network.hpp"
#include "SystemTypes.hpp"
#include "BigError.hpp"
#if _WIN32
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")
#endif

using namespace GameEngine;

void Network::Initialize()
{
#if _WIN32
	WSADATA wsadata;
	int		wsaerr;

	uint16 vreq = MAKEWORD(2, 2);
	wsaerr = WSAStartup(vreq, &wsadata);
	if (wsaerr != 0)
		throw BigError("Error: Winsock DLL not found!");
#endif
}

void Network::Release()
{
#if _WIN32
	WSACleanup();
#endif
}
