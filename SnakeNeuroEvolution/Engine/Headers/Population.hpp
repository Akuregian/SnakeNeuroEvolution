#pragma once
#include "GeneticAlgorithm.hpp"

namespace NeuroEvolution {

	class Population : public NeuroEvolution::GeneticAlgorithm {
	public:
		Population();
		~Population();
		void CreatePopulationOfEntites(const int& pop_size);
		std::vector<std::shared_ptr<NeuroEvolution::Entity>>& TrainEntitiesOnce();
		void TrainEntities();
		void CreateNextGeneration();
		bool isAllEntitiesDead();
		void Update();
		void Results();
		void LoadEntity();

		std::vector<std::shared_ptr<NeuroEvolution::Entity>> _EntityPopulation;
		std::vector<std::shared_ptr<NeuroEvolution::Entity>> _EntityMatingPool;
		
		// TopSnake in Population
		std::shared_ptr<NeuroEvolution::Entity> ReplayEntity;

		unsigned int current_generation;

	};
}