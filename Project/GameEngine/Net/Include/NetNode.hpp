/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETNODE_HPP_
#define NETNODE_HPP_
#include "SystemTypes.hpp"
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

namespace GameEngine
{
	class NetNode
	{
	public:
		virtual void Initialize(uint16 port, strgv ip) = 0;
		virtual void Release() = 0;
		virtual int	 Run() = 0;

	protected:
#if _WIN32
		SOCKET	mSocket;
#else
		int		mSocket;
#endif
		uint16	mPort;
		strg	mIP;
	};
}
#endif