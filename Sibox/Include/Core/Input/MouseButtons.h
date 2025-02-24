#pragma once

using MouseButton = enum MouseButton
{
	PAPI_MOUSE_BUTTON_NONE   = 0,
	PAPI_MOUSE_BUTTON_LEFT   = 1,
	PAPI_MOUSE_BUTTON_MIDDLE = 2,
	PAPI_MOUSE_BUTTON_RIGHT  = 3,
	PAPI_MOUSE_BUTTON_4      = 4,
	PAPI_MOUSE_BUTTON_5      = 5,
	PAPI_MOUSE_BUTTON_6      = 6,
	PAPI_MOUSE_BUTTON_7      = 7,
	PAPI_MOUSE_BUTTON_8      = 8,
	PAPI_MOUSE_BUTTON_COUNT  = PAPI_MOUSE_BUTTON_8,
};

NODISCARD const char* MouseButtonToString(MouseButton button);
