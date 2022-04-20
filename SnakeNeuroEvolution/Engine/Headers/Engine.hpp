/* Engine:  
*			{Neural Network}
*			Creates Neural Network Topology
*			Initliazes Random Weights And Bias'
*			Stores the Forward Propagated Values in 'Neurons'
*			Forward Propagates the Input Vector
* 
*			{Genetic Algorithm}
*			*Must Initialize a Fitness Function*
*			Creates a Population of these 'Networks'
*			Each Generation:
*				Calculates the Fitness Scores
*				Crossover
*				Mutation
*			New Population
*/

// Engine For Neural Network & Genetic Algorithm
#pragma once
#include "Population.hpp"

namespace NeuroEvolution {

	class Engine {
	public:
		Engine();
		~Engine();
		void CreatePopulation(const int& pop_size);
		void TrainPopulation();
		unsigned int& CurrentGeneration() { return m_Population->current_generation; };
		std::shared_ptr<NeuroEvolution::Entity>& TopSnake() { return m_Population->ReplaySnake; };

	private:
		std::shared_ptr<Population> m_Population;
	};
}

