include "msdfgen.lua"

project "msdf-atlas-gen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "On"

	targetdir ("Build/%{prj.name}/" .. outputdir)
	objdir ("Build/%{prj.name}/Intermediates/" .. outputdir)

	files
	{
		"msdf-atlas-gen/msdf-atlas-gen/**.h",
    	"msdf-atlas-gen/msdf-atlas-gen/**.hpp",
    	"msdf-atlas-gen/msdf-atlas-gen/**.cpp"
	}

	removefiles 
	{
		"msdf-atlas-gen/msdf-atlas-gen/artery-font-export.cpp"
	}

	includedirs
	{
		"msdf-atlas-gen/msdf-atlas-gen",
		"msdf-atlas-gen/msdfgen",
		"msdf-atlas-gen/msdfgen/include",
		"msdfgen-custom/"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	links
	{
		"freetype",
		"msdfgen"
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
