/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETDATA_HPP_
#define NETDATA_HPP_
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

#define GAMEENGINE_NET_TCP_DISCONNECTED	0
#define GAMEENGINE_NET_TCP_NOTHING		-1
#define GAMEENGINE_NET_UDP_NOTHING		-1

namespace GameEngine
{
	struct NetClientDataTCP
	{
#if _WIN32
		SOCKET	Socket;
#else
		int		Socket;
#endif
		uint16	Port;
		strg	IP;
	};

	struct NetClientDataUDP
	{
		uint16		Port;
		strg		IP;
		sockaddr_in SocketAddr;
		socklen_t	SocketLen;
	};

	enum class ENetClientStatusTCP
	{
		READY,		// ready to connect
		BUSY,		// already connected
		DEAD		// connection terminated
	};
}
#endif