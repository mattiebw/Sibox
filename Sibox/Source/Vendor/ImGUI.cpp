#include "siboxpch.h"

#ifndef SIBOX_NO_IMGUI
    #define IMGUI_IMPL_OPENGL_LOADER_CUSTOM // We're using GLAD.
    #include "backends/imgui_impl_sdl3.cpp"
    #include "backends/imgui_impl_opengl3.cpp"
    #include <misc/cpp/imgui_stdlib.cpp>
#endif
