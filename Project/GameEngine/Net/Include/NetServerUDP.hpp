/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETSERVERUDP_HPP_
#define NETSERVERUDP_HPP_
#include "SystemTypes.hpp"
#include "NetData.hpp"
#include "NetServer.hpp"

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
