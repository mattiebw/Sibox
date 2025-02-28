#pragma once

#define SPDLOG_EOL ""
#define FMT_UNICODE 0

// Utility functions and types
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>

// Data types
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// GLAD
#include <Vendor/glad/gl.h>

// Steamworks
#include <steam/steam_api.h>

// imgui
#ifndef SIBOX_NO_IMGUI
#include <imgui.h>
#endif

// entt
#include <entt.hpp>

// Core project includes
#include <SDL3/SDL_assert.h>
#include "Core/SiboxCore.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Core/SiboxLog.h"
#include "Core/Buffer.h"
#include "Core/Delegate/Delegate.h"
#include "Core/Utility/MathUtil.h"
#include "Core/Utility/Timer.h"
#include "Core/Transform.h"
#include "Core/UUID.h"
#include "Network/Network.h"

// Platform specific
#ifdef SIBOX_PLATFORM_WINDOWS
#include "Windows.h"
#endif
