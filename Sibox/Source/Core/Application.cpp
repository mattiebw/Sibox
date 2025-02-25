#include "siboxpch.h"
#include "Core/Application.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_timer.h>

#include <steam/steam_api.h>

#include "Audio/AudioManager.h"
#include "Core/Layer.h"
#include "Core/Random.h"
#include "Core/Input/Input.h"
#include "Core/Window.h"
#include "Network/SteamManager.h"
#include "Network/Server.h"
#include "Render/Font.h"
#include "World/World.h"
#include "Render/Renderer.h"

Application *Application::s_Instance = nullptr;

FPSCounter::FPSCounter()
{
	memset(&m_Samples, 0, sizeof(uint16_t) * s_MaxSamples);
}

void FPSCounter::AddSample(uint16_t fps)
{
	m_Samples[m_sampleIndex] = fps;
	m_sampleIndex            = (m_sampleIndex + 1) % s_MaxSamples;
	m_Dirty                  = true;
}

void FPSCounter::RecalculateFPS()
{
	int fpsTotal = 0;
	for (uint16_t m_Sample : m_Samples)
		fpsTotal += m_Sample;
	m_FPS   = static_cast<uint16_t>(fpsTotal / s_MaxSamples);
	m_Dirty = false;
}

Application::Application(const ApplicationSpecification &spec)
	: m_Specification(spec)
{
	Construct();
}

Application::Application(ApplicationSpecification &&spec)
	: m_Specification(std::move(spec))
{
	Construct();
}

Application::~Application()
{
	Shutdown();
	s_Instance = nullptr;
}

void Application::Construct()
{
	SIBOX_ASSERT(s_Instance == nullptr && "There can only be one application instance");
	s_Instance = this;
}

bool Application::Init()
{
	Stopwatch sw;
	
	// First, initialise our core APIs.
	if (!InitSDL())
		return false;

	m_Initialised = true; // Set initialised to true so we can shut down properly.

	m_SavedData.Init(std::filesystem::path(SDL_GetPrefPath("Mattie", "Sibox")) / "Saved");

	// We initialise Steamworks pretty early. This is because it needs to be up and 
	// running before we initialise OpenGL, so that it can hook into device creation
	// to create the overlay.
	// @See: https://partner.steamgames.com/doc/features/overlay
	m_SteamManager = CreateRef<SteamManager>();
	if (!m_SteamManager->Init())
	{
		Shutdown();
		return false;
	}

	Input::Init();
	Random::Init();

	// Create the main window.
	if (HasFrontend())
	{
		m_MainWindow = CreateRef<Window>(WindowSpecification{
			.Title = m_Specification.AppName,
			.Centered = true
		});
		if (!m_MainWindow->IsValid()) // If the window is invalid, we can't continue.
		{
			m_Error = "Failed to create main window";
			Shutdown();
			return false;
		}

		if (!InitRenderer())
		{
			Shutdown();
			return false;
		}

		if (!AudioManager::Init())
		{
			Shutdown();
			return false;
		}
		
		Font::InitFontSystem();
	}

	sw.End();
	SIBOX_TRACE("Application initialisation took {0}ms", sw.GetElapsedMilliseconds());
	
	return true;
}

void Application::BindDelegates()
{
	if (m_MainWindow)
	{
		m_MainWindow->OnClose.BindLambda([](Window *window)
		{
			Get()->m_Running = false;
		});
		m_MainWindow->OnKeyPressed.BindLambda([](Window *window, Scancode scancode, bool repeat)
		{
			if (scancode == SIBOX_KEY_F11)
				window->ToggleFullscreen();

			return false;
		});
	}
}

void Application::Run()
{
	Stopwatch sw;
	
	m_Running = true;

	BindDelegates();

	uint64_t time = SDL_GetPerformanceCounter();
	while (m_Running)
	{
		uint64_t last = time;
		time          = SDL_GetPerformanceCounter();
		m_DeltaTime   = (time - last) / static_cast<double>(SDL_GetPerformanceFrequency());

		m_FPSCounter.AddSample(static_cast<uint16_t>(1.0 / m_DeltaTime));

		static double timeSinceFPSPrinted = 0;
		if (timeSinceFPSPrinted >= 1)
		{
			SIBOX_INFO("FPS: {} (frame time: {:.2f}ms)", m_FPSCounter.GetFPS(), m_FPSCounter.GetAverageFrameTimeMS());
			timeSinceFPSPrinted = 0;
		}
		else
		{
			timeSinceFPSPrinted += m_DeltaTime;
		}

		PreUpdate();
		PollEvents();
		m_SteamManager->Update();
		Update();
		AudioManager::Update();
		Render();
	}

	sw.End();
	SIBOX_TRACE("Application ran for {0}s", sw.GetElapsedSeconds());
}

void Application::Shutdown()
{
	if (!m_Initialised)
		return;

	Stopwatch sw;
	for (const Ref<World> &world : m_Worlds)
		world->Clean();
	m_Worlds.clear();

	for (const Ref<Layer> &layer : m_Layers)
		layer->OnDetach();
	m_Layers.clear();
	
	if (m_SteamManager)
	{
		m_SteamManager->Shutdown();
		m_SteamManager = nullptr;
	}
	
	Font::ShutdownFontSystem();
	Input::Shutdown();
	AudioManager::Shutdown();
	if (m_Renderer)
	{
		m_Renderer->Shutdown();
		m_Renderer = nullptr;
	}
	if (m_MainWindow && m_MainWindow->IsValid())
	{
		m_MainWindow->Destroy();
		m_MainWindow = nullptr;
	}
	ShutdownSDL();
	m_Running     = false;
	m_Initialised = false;
	sw.End();
	SIBOX_TRACE("Application shutdown took {0}ms", sw.GetElapsedMilliseconds());
}

void Application::AddLayer(const Ref<Layer> &layer)
{
	m_Layers.push_back(layer);
	layer->OnAttach();
}

void Application::RemoveLayer(const Ref<Layer> &layer)
{
	layer->OnDetach();
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
		m_Layers.erase(it);
}

Ref<World> Application::AddWorld()
{
	auto world = m_Worlds.emplace_back(CreateRef<World>());
	world->m_NetworkType = m_NetworkType;
	return world;
}

void Application::RemoveWorld(const Ref<World> &world)
{
	auto it = std::find(m_Worlds.begin(), m_Worlds.end(), world);
	if (it != m_Worlds.end())
		m_Worlds.erase(it);
	else
		SIBOX_ERROR("Attempted to remove a world that has not been added!");
}

Ref<World> Application::GetWorldFromPointer(const World *worldPtr)
{
	for (Ref<World> &world : m_Worlds)
		if (world.get() == worldPtr) return world;
	return nullptr;
}

void Application::ShowError(const char *message, const char *title)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr);
}

bool Application::InitSDL()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		m_Error = fmt::format("Failed to initialise SDL: {}", SDL_GetError());
		SIBOX_ERROR("{}", m_Error);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Initialisation Error", m_Error.c_str(), nullptr);
		return false;
	}

	return true;
}

bool Application::InitRenderer()
{
	m_Renderer = CreateRef<Renderer>(RendererSpecification{
		.VSync = false
	});

	if (!m_Renderer->Init(m_MainWindow))
	{
		m_Error = "Failed to initialise renderer; check the log!";
		SIBOX_ERROR("{}", m_Error);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Renderer Initialisation Error", m_Error.c_str(), nullptr);
		return false;
	}

	m_Renderer->DebugUIRenderCallback.BindLambda([]()
	{
		for (const Ref<Layer> &layer : Get()->m_Layers)
		{
			layer->RenderImGUI(Get()->m_DeltaTime);
		}
	});

	return true;
}

bool Application::CreateServer()
{
	if (m_Server)
	{
		m_Server = nullptr; // Destroy our old server.
	}
	
	m_Server = CreateRef<Server>();

	return true;
}

void Application::PreUpdate()
{
	if (HasFrontend())
		Input::PreUpdate();
}

void Application::PollEvents()
{
	if (!HasFrontend())
		return;
	
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		m_Renderer->ProcessSDLEvent(&e);

		switch (e.type)
		{
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			{
				SDL_PropertiesID props = SDL_GetWindowProperties(SDL_GetWindowFromEvent(&e));
				if (Window *window = static_cast<Window*>(SDL_GetPointerProperty(props, "Window", nullptr)))
					window->Close();
			}
			break;
		case SDL_EVENT_WINDOW_RESIZED:
			{
				SDL_PropertiesID props = SDL_GetWindowProperties(SDL_GetWindowFromEvent(&e));
				if (Window *window = static_cast<Window*>(SDL_GetPointerProperty(props, "Window", nullptr)))
				{
					// Skip creating a temporary glm::ivec2
					window->OnResize.Execute(std::move(window), *reinterpret_cast<const glm::ivec2*>(&e.window.data1));
				}
			}
			break;
		case SDL_EVENT_KEY_DOWN:
			{
				Input::ProcessKeyboardInputEvent(e.key);

				Window *    window    = m_MainWindow.get();
				SDL_Window *sdlWindow = SDL_GetWindowFromEvent(&e);
				if (sdlWindow)
				{
					SDL_PropertiesID props = SDL_GetWindowProperties(sdlWindow);
					window                 = static_cast<Window*>(SDL_GetPointerProperty(props, "Window", window));
				}
				SIBOX_ASSERT(window && "No window, but key event?");
				window->OnKeyPressed.Execute(std::move(window), static_cast<Scancode>(e.key.scancode),
				                             std::move(e.key.repeat));
			}
			break;
		case SDL_EVENT_KEY_UP:
			{
				Input::ProcessKeyboardInputEvent(e.key);

				Window *    window    = m_MainWindow.get();
				SDL_Window *sdlWindow = SDL_GetWindowFromEvent(&e);
				if (sdlWindow)
				{
					SDL_PropertiesID props = SDL_GetWindowProperties(sdlWindow);
					window                 = static_cast<Window*>(SDL_GetPointerProperty(props, "Window", window));
				}
				SIBOX_ASSERT(window && "No window, but key event?");
				window->OnKeyReleased.Execute(std::move(window), static_cast<Scancode>(e.key.scancode));
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
			{
				Input::ProcessMouseInputEvent(e.button);

				Window *    window    = m_MainWindow.get();
				SDL_Window *sdlWindow = SDL_GetWindowFromEvent(&e);
				if (sdlWindow)
				{
					SDL_PropertiesID props = SDL_GetWindowProperties(sdlWindow);
					window                 = static_cast<Window*>(SDL_GetPointerProperty(props, "Window", window));
				}
				SIBOX_ASSERT(window && "No window, but mouse event?");

				if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
					window->OnMouseButtonDown.Execute(std::move(window), static_cast<MouseButton>(e.button.button));
				else
					window->OnMouseButtonUp.Execute(std::move(window), static_cast<MouseButton>(e.button.button));
			}
			break;
		case SDL_EVENT_MOUSE_MOTION:
			{
				Input::ProcessMouseMotionEvent(e.motion);

				Window *    window    = m_MainWindow.get();
				SDL_Window *sdlWindow = SDL_GetWindowFromEvent(&e);
				if (sdlWindow)
				{
					SDL_PropertiesID props = SDL_GetWindowProperties(sdlWindow);
					window                 = static_cast<Window*>(SDL_GetPointerProperty(props, "Window", window));
				}

				window->OnMouseMove.Execute(std::move(window), {e.motion.x, e.motion.y},
				                            {e.motion.xrel, e.motion.yrel});
			}
			break;
		default: // Just here to suppress warnings.
			break;
		}
	}
}

void Application::Update()
{
	// Update our layers
	for (Ref<Layer> &layer : m_Layers)
		layer->Update(m_DeltaTime);

	// Tick all the worlds.
	for (Ref<World> &world : m_Worlds)
		world->Tick(m_DeltaTime);
}

void Application::Render()
{
	if (!HasFrontend() || !m_Renderer || !m_MainWindow || !m_MainWindow->IsValid())
		return;

	m_Renderer->BeginFrame();
	// Render our layers
	for (Ref<Layer> &layer : m_Layers)
		layer->Render(m_DeltaTime);
	m_Renderer->Render();
	m_Renderer->EndFrame();
}

void Application::ShutdownSDL()
{
	SIBOX_TRACE("Shutting down SDL");
	SDL_Quit();
}

void Layer::OnAttach()
{
}

void Layer::OnDetach()
{
}

void Layer::Update(double delta)
{
}

void Layer::Render(double delta)
{
}

void Layer::RenderImGUI(double delta)
{
}
