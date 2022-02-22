#include "../Headers/Seed.hpp"

namespace NeuroEvolution {

	std::shared_ptr<Seed> Seed::s_SeedInstance;
	uint32_t Seed::curr_seed;
	std::mt19937 Seed::m_Generator;

	void Seed::Init()
	{
		curr_seed = std::random_device()();
		m_Generator = std::mt19937();

		ENGINE_INIT_INFO("Seed Initialized: {0}", curr_seed);

	}

	uint32_t Seed::GetSeed()
	{
		ENGINE_LOGGER_INFO("Returning Seed: {0}", curr_seed);

		return curr_seed;
	}

	void Seed::setSeed(uint32_t seed)
	{
		curr_seed = seed;

		ENGINE_LOGGER_INFO("Setting Seed: {0}", curr_seed);

		m_Generator.seed(seed);
	}

	void Seed::GenerateNewSeed()
	{
		ENGINE_LOGGER_INFO("Generating new Seed");

		curr_seed = std::random_device()();
		m_Generator = std::mt19937(curr_seed);
	}
}