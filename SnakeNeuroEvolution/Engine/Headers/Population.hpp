#pragma once
#include "GeneticAlgorithm.hpp"

namespace NeuroEvolution {

	class Population : public NeuroEvolution::GeneticAlgorithm {
	public:
		Population();
		~Population();
		void CreatePopulationOfEntites(const int& pop_size);
		void TrainEntities();
		void CreateNextGeneration();

		const int EntityPopulationSize() { return _EntityPopulation.size(); };

		std::vector<std::shared_ptr<NeuroEvolution::Entity>> _EntityPopulation;
		std::vector<std::shared_ptr<NeuroEvolution::Entity>> _EntityMatingPool;

	};
}