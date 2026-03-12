/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#ifndef NETSERVERTCP_HPP_
#define NETSERVERTCP_HPP_
#include "SystemTypes.hpp"
#include "NetData.hpp"
#include "NetServer.hpp"
#include <vector>

namespace GameEngine
{
	class NetServerTCP : public NetServer
	{
	public:
		void	Initialize(uint16 port, strgv ip);
		void	Release();
		int		Run();

		const std::vector<NetClientDataTCP>& Clients();

	private:
		int Connect();
		void Listen();
		void Disconnect(NetClientDataTCP& client);

	protected:
		int			Recv(const NetClientDataTCP& client, char* buffer, int size, int flags = 0);
		int			Send(const NetClientDataTCP& client, const char* buffer, int size, int flags = 0);
		virtual int ListenToClient(const NetClientDataTCP& client) = 0;

	private:
		std::vector<NetClientDataTCP> mClients;
	};
}
#endif
