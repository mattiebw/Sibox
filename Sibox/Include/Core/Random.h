#pragma once

#include <random>

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	NODISCARD FORCEINLINE static f32 Float()
	{
		return s_Distribution(s_RandomEngine);
	}

	NODISCARD FORCEINLINE static f32 Float(f32 min, f32 max)
	{
		return min + s_Distribution(s_RandomEngine) * (max - min);
	}

	NODISCARD FORCEINLINE static int Int()
	{
		return s_IntDistribution(s_RandomEngine);
	}

	NODISCARD FORCEINLINE static int Int(int min, int max)
	{
		return min + s_IntDistribution(s_RandomEngine) % (max - min);
	}

private:
	static std::mt19937                        s_RandomEngine;
	static std::uniform_real_distribution<f32> s_Distribution;
	static std::uniform_int_distribution<int>  s_IntDistribution;
};
