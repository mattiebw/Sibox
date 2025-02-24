﻿#include "papipch.h"
#include "Network/Server.h"

Server::Server()
{
    PAPI_INFO("Creating Server");
    m_Socket = SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
}

Server::~Server()
{
    PAPI_INFO("Destroying Server");
    if (m_Socket != k_HSteamListenSocket_Invalid)
    {
        SteamNetworkingSockets()->CloseListenSocket(m_Socket);
        m_Socket = k_HSteamListenSocket_Invalid;
    }
}
