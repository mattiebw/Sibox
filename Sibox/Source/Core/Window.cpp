#include "siboxpch.h"
#include "Core/Window.h"

#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_video.h>

Window::Window(const WindowSpecification &spec)
{
	m_Specification = spec;

	SIBOX_TRACE("Creating window \"{}\"", m_Specification.Title);

	m_Window = SDL_CreateWindow(m_Specification.Title.c_str(), m_Specification.Size.X, m_Specification.Size.Y,
	                            SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
	if (!m_Window)
	{
		std::string error = fmt::format("Failed to create window: {}", SDL_GetError());
		SIBOX_ERROR("{}", error);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Window Creation Error", error.c_str(), nullptr);
		return;
	}

	m_Specification.OriginalSize = m_Specification.Size;
	if (m_Specification.Centered)
		SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	else
		SDL_SetWindowPosition(m_Window, m_Specification.Position.X, m_Specification.Position.Y);

	SDL_SetWindowMinimumSize(m_Window, m_Specification.MinSize.X, m_Specification.MinSize.Y);
	SDL_SetWindowMaximumSize(m_Window, m_Specification.MaxSize.X, m_Specification.MaxSize.Y);
	SDL_SetWindowResizable(m_Window, m_Specification.Resizable);

	// Associate the Window object with the SDL window, so we can call Window functions from SDL events
	SDL_PropertiesID props = SDL_GetWindowProperties(m_Window);
	SDL_SetPointerProperty(props, "Window", this);

	// MW @gotcha: Don't set up delegates here that capture because the window will likely move around in memory and the this pointer will be invalid
	OnResize.BindLambda([](Window *window, Vector2I size)
	{
		SIBOX_INFO("Window \"{}\" resized to {}", window->GetTitle(), size);
		window->m_Specification.Size = size;
		return false;
	});

	Show();
}

Window::~Window()
{
	if (IsValid())
		Destroy();
}

SDL_GLContext Window::GetGLContext()
{
	if (m_Context == nullptr)
		m_Context = SDL_GL_CreateContext(m_Window);
	return m_Context;
}

void Window::DestroyGLContext()
{
	if (m_Context)
	{
		SDL_GL_DestroyContext(m_Context);
		m_Context = nullptr;
	}
}

void Window::Show()
{
	SIBOX_ASSERT(m_Window && "Window is not initialised");
	SDL_ShowWindow(m_Window);
	m_IsVisible = true;
}

void Window::Hide()
{
	SIBOX_ASSERT(m_Window && "Window is not initialised");
	SDL_HideWindow(m_Window);
	m_IsVisible = false;
}

void Window::Close()
{
	SIBOX_TRACE("Closing window \"{}\"", m_Specification.Title);

	bool shouldDestroy = OnCloseRequested.Execute(this);
	if (!shouldDestroy)
	{
		SIBOX_TRACE("Window \"{}\"'s closure was cancelled by delegates!", m_Specification.Title);
		return;
	}

	OnClose.Execute(this);

	if (m_Window != nullptr)
		SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
}

void Window::Destroy()
{
	OnClose.UnbindAll();
	OnCloseRequested.UnbindAll();
	OnKeyPressed.UnbindAll();
	OnKeyReleased.UnbindAll();
	OnResize.UnbindAll();
	OnMouseMove.UnbindAll();
	OnMouseButtonDown.UnbindAll();
	OnMouseButtonUp.UnbindAll();

	Close();
}

void Window::SetTitle(std::string_view title)
{
	SDL_SetWindowTitle(m_Window, title.data());
	m_Specification.Title = title;
}

void Window::SetFullscreen(bool fullscreen)
{
	SDL_SetWindowFullscreen(m_Window, fullscreen);
	m_Specification.Fullscreen = fullscreen;
}

bool Window::ToggleFullscreen()
{
	SetFullscreen(!m_Specification.Fullscreen);
	return m_Specification.Fullscreen;
}

void Window::LockCursor() const
{
	SDL_SetWindowRelativeMouseMode(m_Window, true);
}

void Window::UnlockCursor() const
{
	SDL_WarpMouseInWindow(m_Window, static_cast<float>(m_Specification.Size.X) / 2,
	                      static_cast<float>(m_Specification.Size.Y) / 2);
	SDL_SetWindowRelativeMouseMode(m_Window, false);
}

bool Window::IsCursorLocked() const
{
	return SDL_GetWindowRelativeMouseMode(m_Window);
}
