#pragma once
#include "Logger.hpp"

namespace NeuroEvolution {

	class Seed
	{
	public:
		static void Init();
		static inline std::shared_ptr<Seed>& GetInstance() { return s_SeedInstance; };

		static uint32_t GetSeed();
		static void setSeed(uint32_t seed);
		static void GenerateNewSeed();
		static std::mt19937 Generator() { return m_Generator; };

		static uint32_t curr_seed;
		static std::mt19937 m_Generator;

	private:
		static std::shared_ptr<Seed> s_SeedInstance;

	};
}

#define NEW_SEED NeuroEvolution::Seed::GetInstance()->GenerateNewSeed()
#define SET_SEED(...) NeuroEvolution::Seed::GetInstance()->setSeed(__VA_ARGS__)
#define CURRENT_SEED ENGINE_RESULTS_LOGGER(NeuroEvolution::Seed::GetInstance()->curr_seed)