#pragma once

struct Lobby
{
	std::string Name;
};

class SteamManager
{
public:
	bool Init();
	void Update();
	void Shutdown();

	void SetRequestingLobbies();
	void SetNotRequestingLobbies();
	std::vector<Lobby> GetLobbies() { return m_Lobbies; };

	FORCEINLINE bool HasRelayAccess() { return SteamNetworkingUtils()->GetRelayNetworkStatus(nullptr) == k_ESteamNetworkingAvailability_Current; }

	STEAM_CALLBACK(SteamManager, OnScreenshot, ScreenshotReady_t);
	STEAM_CALLBACK(SteamManager, OnRelayNetworkStatusChanged, SteamRelayNetworkStatus_t);

private:
	std::vector<Lobby> m_Lobbies;
	bool m_RequestingLobbies = false;
	bool m_HasRelayAccess = false;
	
	bool m_SteamworksInitialised = false;
};
