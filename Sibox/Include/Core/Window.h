#pragma once

#include <SDL3/SDL_video.h>

#include "Input/MouseButtons.h"
#include "Input/Scancode.h"

struct WindowSpecification
{
	std::string Title = "Window";

	// (0, 0) for default position. Ignored if Centered is true.
	glm::ivec2 Position     = {0, 0};
	glm::ivec2 Size         = {800, 600}, MinSize = {0, 0}, MaxSize = {0, 0};
	glm::ivec2 OriginalSize = Size;
	bool       Centered     = true;
	bool       Fullscreen   = false;

	bool Resizable = true;
};

class Window
{
public:
	Window(const WindowSpecification &spec);
	~Window();

	NODISCARD SDL_GLContext                          GetGLContext();
	NODISCARD FORCEINLINE SDL_Window*                GetHandle() const { return m_Window; }
	NODISCARD FORCEINLINE const WindowSpecification& GetSpec() const { return m_Specification; }

	void DestroyGLContext();

	void Show();
	void Hide();

	void Close();
	void Destroy();

	NODISCARD FORCEINLINE bool IsVisible() const { return m_IsVisible; };
	NODISCARD FORCEINLINE bool IsValid() const { return m_Window != nullptr; }

	void                              SetTitle(std::string_view title);
	NODISCARD FORCEINLINE std::string GetTitle() const { return m_Specification.Title; }

	void                       SetFullscreen(bool fullscreen);
	bool                       ToggleFullscreen();
	NODISCARD FORCEINLINE bool IsFullscreen() { return m_Specification.Fullscreen; };

	CascadingMulticastDelegate<false, Window*, const glm::ivec2&>                  OnResize;
	CascadingMulticastDelegate<false, Window*>                                     OnCloseRequested;
	MulticastDelegate<Window*>                                                     OnClose;
	CascadingMulticastDelegate<false, Window*, Scancode, bool>                     OnKeyPressed;
	CascadingMulticastDelegate<false, Window*, Scancode>                           OnKeyReleased;
	CascadingMulticastDelegate<false, Window*, MouseButton>                        OnMouseButtonDown;
	CascadingMulticastDelegate<false, Window*, MouseButton>                        OnMouseButtonUp;
	CascadingMulticastDelegate<false, Window*, const glm::vec2&, const glm::vec2&> OnMouseMove; // Position, Delta

protected:
	bool                m_IsVisible = false;
	SDL_Window *        m_Window    = nullptr;
	SDL_GLContext       m_Context   = nullptr;
	WindowSpecification m_Specification;
};
