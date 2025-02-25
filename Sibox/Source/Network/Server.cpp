#include "siboxpch.h"
#include "Network/Server.h"

Server::Server()
{
    SIBOX_INFO("Creating Server");
    m_Socket = SteamNetworkingSockets()->CreateListenSocketP2P(0, 0, nullptr);
}

Server::~Server()
{
    SIBOX_INFO("Destroying Server");
    if (m_Socket != k_HSteamListenSocket_Invalid)
    {
        SteamNetworkingSockets()->CloseListenSocket(m_Socket);
        m_Socket = k_HSteamListenSocket_Invalid;
    }
}
