#include "../Headers/Seed.hpp"

namespace NeuroEvolution {

	std::shared_ptr<Seed> Seed::s_SeedInstance;
	uint32_t Seed::curr_seed;
	std::mt19937 Seed::m_Generator;

	void Seed::Init()
	{
		curr_seed = std::random_device()();
		m_Generator = std::mt19937();
	}

	uint32_t Seed::GetSeed()
	{
		return curr_seed;
	}

	void Seed::setSeed(uint32_t seed)
	{
		curr_seed = seed;
		m_Generator.seed(curr_seed);
	}

	void Seed::GenerateNewSeed()
	{
		curr_seed = std::random_device()();
		m_Generator = std::mt19937(curr_seed);
	}
}