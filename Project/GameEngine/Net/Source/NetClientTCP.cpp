/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "NetClientTCP.hpp"
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

void NetClientTCP::Initialize(uint16 port, strgv ip)
{
	mSocket = INVALID_SOCKET;
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	NetClient::Initialize(port, ip);
}

int NetClientTCP::TryConnecting()
{
	connect(mSocket, (SOCKADDR*)&mService, sizeof(mService));

	fd_set writeSet;
	FD_ZERO(&writeSet);
	FD_SET(mSocket, &writeSet);

	fd_set exceptSet;
	FD_ZERO(&exceptSet);
	FD_SET(mSocket, &exceptSet);

	TIMEVAL timeout = { 0, 0 }; // non-blocking check

	int result = select(0, nullptr, &writeSet, &exceptSet, &timeout);
	if (result > 0)
	{
		int err = 0;
		int len = sizeof(err);
		getsockopt(mSocket, SOL_SOCKET, SO_ERROR, (char*)&err, &len);
		if (err == 0)
			mStatus = ENetClientStatusTCP::BUSY;
		else return err;
	}
	return GAMEENGINE_NET_TCP_DISCONNECTED;
}

int NetClientTCP::Run()
{
	switch (mStatus)
	{
	case ENetClientStatusTCP::READY:
	{
		return TryConnecting();
	}break;
	case ENetClientStatusTCP::BUSY:
	{
		const int code = ListenToServer();
		if((code == GAMEENGINE_NET_TCP_DISCONNECTED) ||
#if _WIN32
			(code == GAMEENGINE_NET_TCP_NOTHING && WSAGetLastError() == WSAECONNRESET))
#else
			(code == GAMEENGINE_NET_TCP_NOTHING && errno == WSAECONNRESET))
#endif
			mStatus = ENetClientStatusTCP::DEAD;
		return code;
	}break;
	}
	return GAMEENGINE_NET_TCP_NOTHING;
}

ENetClientStatusTCP NetClientTCP::Status()
{
	return mStatus;
}

int NetClientTCP::Recv(char* buffer, int size, int flags)
{
	return recv(mSocket, buffer, size, flags);
}

int NetClientTCP::Send(const char* buffer, int size, int flags)
{
	return send(mSocket, buffer, size, flags);
}
