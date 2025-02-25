#include "siboxpch.h"

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_main.h>

#include "Core/Application.h"
#include "Game/SiboxLayer.h"

bool g_ShouldRestart = false;

int main(int argc, char *argv[])
{
	InitLog(SDL_GetPrefPath("Mattie", "Sibox"));

	do
	{
		SIBOX_INFO("Welcome to Sibox!");

		Application application({.AppName = "Sibox", .Version = SemVer(1, 0, 0)});
		if (application.Init())
		{
			application.CreateLayer<SiboxLayer>();
			application.Run();
			application.Shutdown();
		}
		else
		{
			g_ShouldRestart = false;
			return -1;
		}
	}
	while (g_ShouldRestart);

	return 0;
}
