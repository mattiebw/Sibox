#pragma once

// An enum representing the current network type of the application.
enum class NetworkType : uint8_t
{
    Standalone, // There is no server or client - networking is disabled.
    Client, // We are a client - there is no local server.
    DedicatedServer, // We are a sever - there is no local client.
    ListenServer, // We are a server and also a client to that server.
};

// An enum representing the current state of an entity within the networking system.
enum class EntityNetworkType : uint8_t
{
    LocalOnly, // Only exists on the client. For example, particle systems. May have been created by the server, but isn't synced at all.
    LocalReplicated, // Exists on the server, and we are the server. We may not own it, but we do have authority.
    RemoteOwned, // Exists on the server, but is owned by this client, so we have special powers over it (such as destroying it).
    RemoteProxy, // Exists on the server, and we have no power to change it, but we receive updates on it from the server.
};

inline const char* NetworkTypeToString(NetworkType networkType)
{
    switch (networkType)
    {
        case NetworkType::Standalone: return "Standalone";
        case NetworkType::Client: return "Client";
        case NetworkType::DedicatedServer: return "Dedicated Server";
        case NetworkType::ListenServer: return "Listen Server";
    }

    SIBOX_ASSERT(false && "Unknown network type");
    return "Unknown";
}

inline const char* EntityNetworkTypeToString(EntityNetworkType entityNetworkType)
{
    switch (entityNetworkType)
    {
        case EntityNetworkType::LocalOnly: return "Local Only";
        case EntityNetworkType::RemoteOwned: return "Remote Owned";
        case EntityNetworkType::RemoteProxy: return "Remote Proxy";
    }

    SIBOX_ASSERT(false && "Unknown entity network type");
    return "Unknown";
}
