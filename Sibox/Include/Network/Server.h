#pragma once

class Server
{
public:
    Server();
    ~Server();

private:
    HSteamListenSocket m_Socket;
};
