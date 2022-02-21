#include "../Headers/Seed.hpp"

namespace NeuroEvolution {

	Seed::Seed()
	{
		ENGINE_INIT_WARN("Seed Init");
	}

	Seed::~Seed()
	{
		ENGINE_INIT_WARN("Seed Object Destroyed");
	}

	uint32_t Seed::GetSeed()
	{
		return curr_seed;
	}

	void Seed::setSeed(uint32_t seed)
	{
		ENGINE_LOGGER_INFO("Setting Seed");
		curr_seed = seed;
		m_Generator.seed(seed);
	}

	void Seed::GenerateNewSeed()
	{
		ENGINE_LOGGER_INFO("Generating new Seed");
		curr_seed = std::random_device()();
		m_Generator = std::mt19937(curr_seed);
	}

}