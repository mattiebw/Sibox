#pragma once

#include <random>

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	NODISCARD FORCEINLINE static float Float()
	{
		return s_Distribution(s_RandomEngine);
	}

	NODISCARD FORCEINLINE static float Float(float min, float max)
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
	static std::mt19937                          s_RandomEngine;
	static std::uniform_real_distribution<float> s_Distribution;
	static std::uniform_int_distribution<int>    s_IntDistribution;
};
