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

	NODISCARD FORCEINLINE f64 GetElapsedMilliseconds() const
	{
		u64 end = m_End;
		if (end == 0)
			end = SDL_GetPerformanceCounter();
		return static_cast<f64>((end - m_Begin) * 1000) / static_cast<f64>(SDL_GetPerformanceFrequency());
	}

	NODISCARD FORCEINLINE f64 GetElapsedSeconds() const
	{
		u64 end = m_End;
		if (end == 0)
			end = SDL_GetPerformanceCounter();
		return static_cast<f64>(end - m_Begin) / static_cast<f64>(SDL_GetPerformanceFrequency());
	}

	NODISCARD FORCEINLINE u64 GetElapsedTicks() const
	{
		u64 end = m_End;
		if (end == 0)
			end = SDL_GetPerformanceCounter();
		return end - m_Begin;
	}

	NODISCARD FORCEINLINE u64 GetElapsedMicroseconds() const
	{
		u64 end = m_End;
		if (end == 0)
			end = SDL_GetPerformanceCounter();
		return (end - m_Begin) * 1000000 / SDL_GetPerformanceFrequency();
	}

private:
	u64 m_Begin, m_End;
};
