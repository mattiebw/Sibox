include "freetype.lua"

project "msdfgen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "On"

	targetdir ("Build/%{prj.name}/" .. outputdir)
	objdir ("Build/%{prj.name}/Intermediates/" .. outputdir)

	files
	{
		"msdf-atlas-gen/msdfgen/core/**.h",
		"msdf-atlas-gen/msdfgen/core/**.hpp",
		"msdf-atlas-gen/msdfgen/core/**.cpp",
		"msdf-atlas-gen/msdfgen/ext/**.h",
		"msdf-atlas-gen/msdfgen/ext/**.hpp",
		"msdf-atlas-gen/msdfgen/ext/**.cpp",
		"msdf-atlas-gen/msdfgen/lib/**.cpp",
		"msdf-atlas-gen/msdfgen/include/**.h"
	}

	includedirs
	{
		"msdf-atlas-gen/msdfgen/include",
		"freetype/include",
		"msdfgen-custom/"
	}

	defines
	{
		"MSDFGEN_USE_CPP11"
	}

	links
	{
		"freetype"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

    filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"

	filter "platforms:Win64"
		system "Windows"
		architecture "x64"
	
	filter "platforms:Linux64"
		system "linux"
		architecture "x64"
