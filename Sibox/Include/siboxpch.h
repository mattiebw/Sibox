#pragma once

#define SPDLOG_EOL ""

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

// GLM
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"

// GLAD
#include <Vendor/glad/gl.h>

// Steamworks
#include <steam/steam_api.h>

// imgui
#ifndef PAPI_NO_IMGUI
#include <imgui.h>
#endif

// Core project includes
#include <SDL3/SDL_assert.h>
#include "Core/PAPICore.h"
#include "Core/PAPILog.h"
#include "Core/Buffer.h"
#include "Core/Delegate/Delegate.h"
#include "Core/Utility/MathUtil.h"
#include "Core/Utility/Timer.h"
#include "Core/Transform.h"
#include "Core/UUID.h"
#include "Network/Network.h"

// Platform specific
#ifdef PAPI_PLATFORM_WINDOWS
#include "Windows.h"
#endif
