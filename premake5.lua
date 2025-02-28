workspace "Sibox"
	configurations { "Debug", "Release", "Dist" }
	platforms { "Win64", "Linux" }
	startproject "Sibox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "Sibox/Vendor/spdlog/include" -- MW @todo @build @perf: Consider moving to compiled spdlog
IncludeDir["SDL"] = "Sibox/Vendor/SDL/include"
IncludeDir["imgui"] = "Sibox/Vendor/imgui/"
IncludeDir["msdfatlasgen"] = "Sibox/Vendor/msdf-atlas-gen/msdf-atlas-gen/"
IncludeDir["msdfgen"] = "Sibox/Vendor/msdf-atlas-gen/msdfgen/"
IncludeDir["glm"] = "Sibox/Vendor/glm/Include"
IncludeDir["stb"] = "Sibox/Vendor/stb"
IncludeDir["steamworks"] = "Sibox/Vendor/Steamworks/Include"
IncludeDir["fmod"] = "Sibox/Vendor/FMOD/Include"
IncludeDir["entt"] = "Sibox/Vendor/entt"

group "Vendor"
include "Sibox/Vendor/imgui.lua"
include "Sibox/Vendor/msdf-atlas-gen.lua"
group ""

project "Sibox"
	cppdialect "C++20"
	kind "ConsoleApp"
	staticruntime "On"
	language "C++"
	characterset "Unicode"
	location "Sibox"
	targetdir ("Build/%{prj.name}/" .. outputdir)
	objdir ("Build/%{prj.name}/Intermediates/" .. outputdir)

	usestandardpreprocessor 'On'
	pchheader("siboxpch.h")
	pchsource "Sibox/Source/siboxpch.cpp"

	vpaths {
		["Include"] = {"Sibox/Include/**.h", "Sibox/Include/**.hpp"},
		["Source"] = {"Sibox/Source/**.cpp", "Sibox/Source/**.c"},
	}

	files { 
		"Sibox/Include/**.h", "Sibox/Include/**.hpp", 
		"Sibox/Source/**.cpp", "Sibox/Source/**.c",
		"Sibox/Content/**",
		"TODO.md", "README.md",
	}

	includedirs 
	{ 
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.SDL}",
		"%{IncludeDir.imgui}",
		-- "%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.msdfgen}",
		"Sibox/Vendor/msdfgen-custom/",
		"%{IncludeDir.msdfatlasgen}",
		"%{IncludeDir.steamworks}",
		"%{IncludeDir.fmod}",
		"%{IncludeDir.entt}",

		"Sibox/Include"
	}

	filter "system:windows"
		libdirs 
		{
			"Sibox/Vendor/SDL/Lib/Win64",
			"Sibox/Vendor/Steamworks/Lib/Win64",
			"Sibox/Vendor/FMOD/Lib/Win64"
		}

		links 
		{
			"steam_api64",

			-- FMOD
			"fmod_vc",
			"fmodstudio_vc",
			"fsbank_vc",
		}

	filter "system:linux"
		libdirs 
		{
			"Sibox/Vendor/SDL/Lib/Linux64",
			"Sibox/Vendor/Steamworks/Lib/Linux64",
			"Sibox/Vendor/FMOD/Lib/Linux64"
		}
		
		links 
		{
			"steam_api",

			-- FMOD
			"fmod",
			"fmodstudio",
			"fsbank",
		}

	filter {}

	-- Linking order matters with gcc! freetype needs to be at the bottom.
	-- MW @todo: Understand linking order, consider using 'linkgroups "On"' to avoid linking order issues at the cost of link time.
	-- https://premake.github.io/docs/linkgroups/
	links
	{
		"imgui",
		"SDL3",

		-- MSDF
		"msdf-atlas-gen",
		"msdfgen",
		"freetype",
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		prebuildcommands { "call \"../Scripts/RunPreprocessor.bat\" " .. outputdir }

	filter "system:linux"
		prebuildcommands { "../Scripts/RunPreprocessor.sh " .. outputdir }
		postbuildcommands { "{COPYFILE} \"./RunSibox.sh\" \"" .. path.getdirectory("path") .. "/../Build/%{prj.name}/" .. outputdir .. "/\"" }

	filter { "system:linux", "files:Sibox/Source/Vendor/stb.cpp" }
		optimize "On" -- MW @hack: stb doesn't compile properly with GCC without optimizations (@credit https://git.suyu.dev/suyu/suyu/pulls/63)

os.mkdir("Sibox/Source")
os.mkdir("Sibox/Include")

filter "configurations:Debug"
	defines { "SIBOX_DEBUG", "SIBOX_ENABLE_ASSERTS", "SIBOX_GL_DEBUG" }
	symbols "On"
	runtime "Debug"

filter "configurations:Release"
	defines { "SIBOX_RELEASE", "SIBOX_ENABLE_ASSERTS", "SIBOX_GL_DEBUG" }
	optimize "On"
	symbols "On"
	runtime "Release"

filter "configurations:Dist"
	defines { "SIBOX_DIST" }
	kind "WindowedApp"
	optimize "On"
	symbols "Off"
	runtime "Release"

filter "system:windows"
	systemversion "latest"
	defines { "SIBOX_PLATFORM_WINDOWS" }

	links
	{
		"version",
		"winmm",
		"Imm32",
		"Cfgmgr32",
		"Setupapi"
	}

filter "system:linux"
	defines { "SIBOX_PLATFORM_LINUX" }

filter "platforms:Win64"
	system "Windows"
	architecture "x64"

filter "platforms:Linux"
	buildoptions { "-static-libstdc++" }
	system "linux"
	architecture "x64"
