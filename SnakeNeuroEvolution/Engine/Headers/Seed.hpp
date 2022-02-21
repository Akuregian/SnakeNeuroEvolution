#pragma once
#include "Logger.hpp"

namespace NeuroEvolution {

	class Seed
	{
	public:
		Seed();
		~Seed();

		uint32_t GetSeed();
		void setSeed(uint32_t seed);
		void GenerateNewSeed();

	private:
		uint32_t curr_seed;
		std::mt19937 m_Generator;
	};
}