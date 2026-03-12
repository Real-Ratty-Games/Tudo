/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "NetClientTCP.hpp"
#include "BigError.hpp"
#include "Network.hpp"

using namespace GameEngine;

void NetClientTCP::Initialize(uint16 port, strgv ip)
{
    mSocket = GAMEENGINE_NET_SOCKET_INVALID;
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	NetClient::Initialize(port, ip);
}

int NetClientTCP::TryConnecting()
{
    connect(mSocket, (NetSockaddr*)&mService, sizeof(mService));

	fd_set writeSet;
	FD_ZERO(&writeSet);
	FD_SET(mSocket, &writeSet);

	fd_set exceptSet;
	FD_ZERO(&exceptSet);
	FD_SET(mSocket, &exceptSet);

    NetTimeval timeout = { 0, 0 }; // non-blocking check

	int result = select(0, nullptr, &writeSet, &exceptSet, &timeout);
	if (result > 0)
	{
		int err = 0;
#if _WIN32
        int len = sizeof(err);
        getsockopt(mSocket, SOL_SOCKET, SO_ERROR, (char*)&err, &len);
#else
        socklen_t len = sizeof(err);
        getsockopt(mSocket, SOL_SOCKET, SO_ERROR, &err, &len);
#endif
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
			(code == GAMEENGINE_NET_TCP_NOTHING && Network::GetError() == GAMEENGINE_NET_CONNRESET))
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
