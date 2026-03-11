/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETSERVERUDP_HPP_
#define NETSERVERUDP_HPP_
#include "SystemTypes.hpp"
#include "NetData.hpp"
#include "NetServer.hpp"
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
	class NetServerUDP : public NetServer
	{
	public:
		void	Initialize(uint16 port, strgv ip);
		void	Release();
		int		Run();

	protected:
		int			Recv(NetClientDataUDP& client, char* buffer, int size, int flags = 0);
		int			Send(const NetClientDataUDP& client, const char* buffer, int size, int flags = 0);
		virtual int ListenToClient() = 0;
	};
}
#endif