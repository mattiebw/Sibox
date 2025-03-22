#include "siboxpch.h"

#include <random>

namespace
{
	std::random_device                 s_RandomDevice;
	std::mt19937_64                    s_Engine(s_RandomDevice());
	std::uniform_int_distribution<u64> s_UniformDistribution;
}

UUID::UUID()
	: m_UUID(s_UniformDistribution(s_Engine))
{
}

UUID::UUID(u64 uuid)
	: m_UUID(uuid)
{
}
