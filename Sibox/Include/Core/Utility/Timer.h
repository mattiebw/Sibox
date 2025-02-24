#pragma once

#include <SDL3/SDL_timer.h>

class Stopwatch
{
public:
	Stopwatch()
	{
		Restart();
	}

	FORCEINLINE void Restart()
	{
		m_End   = 0;
		m_Begin = SDL_GetPerformanceCounter();
	}

	FORCEINLINE void End()
	{
		m_End = SDL_GetPerformanceCounter();
	}

	NODISCARD FORCEINLINE double GetElapsedMilliseconds() const
	{
		uint64_t end = m_End;
		if (end == 0)
			end = SDL_GetPerformanceCounter();
		return static_cast<double>((end - m_Begin) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency());
	}

	NODISCARD FORCEINLINE double GetElapsedSeconds() const
	{
		uint64_t end = m_End;
		if (end == 0)
			end = SDL_GetPerformanceCounter();
		return static_cast<double>(end - m_Begin) / static_cast<double>(SDL_GetPerformanceFrequency());
	}

	NODISCARD FORCEINLINE uint64_t GetElapsedTicks() const
	{
		uint64_t end = m_End;
		if (end == 0)
			end = SDL_GetPerformanceCounter();
		return end - m_Begin;
	}

	NODISCARD FORCEINLINE uint64_t GetElapsedMicroseconds() const
	{
		uint64_t end = m_End;
		if (end == 0)
			end = SDL_GetPerformanceCounter();
		return static_cast<uint64_t>((end - m_Begin) * 1000000) / SDL_GetPerformanceFrequency();
	}

private:
	uint64_t m_Begin, m_End;
};
