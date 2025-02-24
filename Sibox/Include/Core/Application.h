#pragma once

#include "Layer.h"
#include "SavedDataManager.h"
#include "Render/Renderer.h"

class Server;
class Client;
class SteamManager;
class QuadBatch;
class Layer;
class World;
class Renderer;
class Window;

struct ApplicationSpecification
{
	std::string AppName = "App";
	SemVer      Version = SemVer(1, 0, 0);
};

class FPSCounter
{
public:
	FPSCounter();

	void                           AddSample(uint16_t fps);
	NODISCARD FORCEINLINE uint16_t GetFPS()
	{
		if (m_Dirty) RecalculateFPS();
		return m_FPS;
	};
	NODISCARD FORCEINLINE double GetAverageFrameTimeMS() { return 1000.0 / GetFPS(); }

private:
	void RecalculateFPS();

	constexpr static uint16_t s_MaxSamples = 200;

	uint16_t m_Samples[s_MaxSamples];
	uint8_t  m_sampleIndex = 0;
	uint16_t m_FPS         = 0;
	bool     m_Dirty       = true;
};

class Application
{
public:
	Application(const ApplicationSpecification &spec);
	Application(ApplicationSpecification &&spec);
	~Application();

	void Construct();
	bool Init();
	void BindDelegates();
	void Run();
	void Shutdown();

	template <typename T>
	Ref<Layer> CreateLayer()
	{
		Ref<Layer> layer = m_Layers.emplace_back(CreateRef<T>());
		layer->OnAttach();
		return layer;
	}

	void AddLayer(const Ref<Layer> &layer);
	void RemoveLayer(const Ref<Layer> &layer);

	Ref<World> AddWorld();
	void       RemoveWorld(const Ref<World> &world);
	Ref<World> GetWorldFromPointer(const World *world);

	bool CreateServer();
	bool JoinServer();
	void DisconnectFromServer();
	
	void ShowError(const char *message, const char *title = "Error");

	NODISCARD static FORCEINLINE Application*          Get() { return s_Instance; }
	NODISCARD static FORCEINLINE const Ref<Renderer>&  GetRenderer() { return s_Instance->m_Renderer; }
	NODISCARD static FORCEINLINE const Ref<QuadBatch>& GetQuadRenderer()
	{
		return s_Instance->m_Renderer->GetQuadRenderer();
	}

	NODISCARD static FORCEINLINE const TilemapRenderer& GetTilemapRenderer()
	{
		return s_Instance->m_Renderer->GetTilemapRenderer();
	}

	NODISCARD static FORCEINLINE TextRenderer& GetTextRenderer()
	{
		return s_Instance->m_Renderer->GetTextRenderer();
	}

	NODISCARD static FORCEINLINE uint16_t GetFPS() { return s_Instance->m_FPSCounter.GetFPS(); };

	NODISCARD static FORCEINLINE SavedDataManager& GetSavedDataManager()
	{
		return s_Instance->m_SavedData;
	}

	NODISCARD FORCEINLINE bool             IsRunning() const { return m_Running; }
	NODISCARD FORCEINLINE bool             IsInitialised() const { return m_Initialised; }
	NODISCARD FORCEINLINE std::string_view GetError() const { return m_Error; }
	NODISCARD FORCEINLINE double           GetDeltaTime() const { return m_DeltaTime; }
	NODISCARD FORCEINLINE NetworkType      GetNetworkType() const { return m_NetworkType; }
	NODISCARD FORCEINLINE bool             HasFrontend() const { return m_NetworkType != NetworkType::DedicatedServer; }

protected:
	bool InitSDL();
	bool InitRenderer();

	void PreUpdate();
	void PollEvents();
	void Update();
	void Render();

	static void ShutdownSDL();

	static Application *s_Instance;

	FPSCounter               m_FPSCounter;
	ApplicationSpecification m_Specification;
	SavedDataManager         m_SavedData;
	std::vector<Ref<World>>  m_Worlds;
	std::vector<Ref<Layer>>  m_Layers;
	std::string              m_Error;
	Ref<Window>              m_MainWindow;
	Ref<Renderer>            m_Renderer;
	Ref<SteamManager>        m_SteamManager;
	Ref<Client>              m_Client;
	Ref<Server>              m_Server;
	double                   m_DeltaTime = 0;
	bool                     m_Running     = false;
	bool                     m_Initialised = false;
	NetworkType              m_NetworkType = NetworkType::Standalone;
};
