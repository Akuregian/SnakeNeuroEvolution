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

		bool isAllSnakesDead();
		void Update();
		const int EntityPopulationSize() { return _EntityPopulation.size(); };
		void Results();
		void LoadSnake();

		std::vector<std::shared_ptr<NeuroEvolution::Entity>> _EntityPopulation;
		std::vector<std::shared_ptr<NeuroEvolution::Entity>> _EntityMatingPool;
		
		// TopSnake in Population
		std::shared_ptr<NeuroEvolution::Entity> ReplaySnake;

		unsigned int current_generation;

	};
}