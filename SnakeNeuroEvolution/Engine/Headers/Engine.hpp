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

	// Forward Declarations
	class Population;

	class Engine {
	public:
		Engine();
		~Engine();
		void CreatePopulation(const int& pop_size);
		void TrainPopulation();
		void CreateNextGeneration();

	private:
		std::shared_ptr<Population> _Population;
	};
}

