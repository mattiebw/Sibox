#include "siboxpch.h"
#include "Core/Input/Input.h"

bool      Input::s_Keys[SIBOX_KEY_COUNT];
bool      Input::s_PreviousKeys[SIBOX_KEY_COUNT];
bool      Input::s_MouseButtons[SIBOX_MOUSE_BUTTON_COUNT];
bool      Input::s_PreviousMouseButtons[SIBOX_MOUSE_BUTTON_COUNT];
glm::vec2 Input::s_MousePosition, Input::s_MouseDelta;
bool      Input::s_ImGuiHasKeyboardFocus = false;
bool      Input::s_ImGuiHasMouseFocus    = false;

void Input::Init()
{
	memset(s_Keys, 0, SIBOX_KEY_COUNT * sizeof(bool));
	memset(s_PreviousKeys, 0, SIBOX_KEY_COUNT * sizeof(bool));
	memset(s_MouseButtons, 0, SIBOX_MOUSE_BUTTON_COUNT * sizeof(bool));
	memset(s_PreviousMouseButtons, 0, SIBOX_MOUSE_BUTTON_COUNT * sizeof(bool));
}

void Input::Shutdown()
{
	// Nothing to do here for now, but if we add static delegates or something, we might need to clean up.
}

void Input::ProcessKeyboardInputEvent(const SDL_KeyboardEvent &event)
{
	s_Keys[event.scancode] = event.down;
}

void Input::ProcessMouseInputEvent(const SDL_MouseButtonEvent &event)
{
	s_MouseButtons[event.button] = event.down;
}

void Input::ProcessMouseMotionEvent(const SDL_MouseMotionEvent &event)
{
	s_MousePosition = {event.x, event.y};
	s_MouseDelta    = {event.xrel, event.yrel};
}

const char* MouseButtonToString(MouseButton button)
{
	switch (button)
	{
	case SIBOX_MOUSE_BUTTON_NONE:
		return "None";
	case SIBOX_MOUSE_BUTTON_LEFT:
		return "Left Mouse Button";
	case SIBOX_MOUSE_BUTTON_MIDDLE:
		return "Middle Mouse Button";
	case SIBOX_MOUSE_BUTTON_RIGHT:
		return "Right Mouse Button";
	case SIBOX_MOUSE_BUTTON_4:
		return "Mouse Button 4";
	case SIBOX_MOUSE_BUTTON_5:
		return "Mouse Button 5";
	case SIBOX_MOUSE_BUTTON_6:
		return "Mouse Button 6";
	case SIBOX_MOUSE_BUTTON_7:
		return "Mouse Button 7";
	case SIBOX_MOUSE_BUTTON_8:
		return "Mouse Button 8";
	default:
		SIBOX_ERROR("Invalid mouse button ({0}) in MouseButtonToString()", static_cast<int>(button));
		return "Unknown Mouse Button";
	}
}

const char* ScancodeToString(MouseButton button)
{
	return SDL_GetScancodeName(static_cast<SDL_Scancode>(button));
}
