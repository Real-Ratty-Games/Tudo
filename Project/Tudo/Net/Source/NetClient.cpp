/*======================================================
	Copyright (c) 2026 Real Ratty Games.
	Created by Norbert Gerberg.
======================================================*/
#include "NetClient.hpp"
#include "Logger.hpp"
#include "Network.hpp"

using namespace Tudo;

NetClient::~NetClient()
{
    Network::CloseSocket(mSocket);
}

void NetClient::Initialize(uint16 port, strgv ip)
{
    mPort    = port;
    mIP      = ip;

    if (mSocket == TUDO_NET_SOCKET_INVALID)
        Logger::Log("NetClient::Initialize", "Failed creating client socket: " + std::to_string(Network::GetError()), ELogType::LERROR);
    
    Network::SetSocketNB(mSocket);
    Network::Pton(mService, ip);

    mService.sin_family = AF_INET;
    mService.sin_port = htons(port);

    Logger::Log("NetClient successfully initialized!");
}
