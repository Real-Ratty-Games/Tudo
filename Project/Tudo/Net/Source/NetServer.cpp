/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "NetServer.hpp"
#include "Logger.hpp"
#include "Network.hpp"

using namespace Tudo;

void NetServer::Initialize(uint16 port, strgv ip)
{
	mPort	= port;
	mIP		= ip;

    if (mSocket == TUDO_NET_SOCKET_INVALID)
		Logger::Log("NetServer::Initialize", "Failed creating server socket: " + std::to_string(Network::GetError()), ELogType::LERROR);
    
    Network::SetSocketNB(mSocket);
    
	sockaddr_in service;
	service.sin_family = AF_INET;
    service.sin_port = htons(port);
    
    Network::Pton(service, ip);

	if (bind(mSocket, (NetSockaddr*)&service, sizeof(service)) == TUDO_NET_SOCKET_INVALID)
		Logger::Log("NetServer::Initialize", "Failed binding server: " + std::to_string(Network::GetError()), ELogType::LERROR);

	Logger::Log("NetServer successfully initialized!");
}
