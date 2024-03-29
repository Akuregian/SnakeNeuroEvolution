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

		// Create the Population Entities
		void CreatePopulation();

		// Trains Entire Generation until all Entitis Die
		void TrainPopulation();

		// Call Population->CreateNextGeneration()
		void CreateNextGeneration() { m_Population->CreateNextGeneration(); };

		void CreatePopulationOfElites();

		// Train Population Once then Send positions to Wrapper
		std::vector<std::shared_ptr<NeuroEvolution::Entity>>& TrainOnce();
		
		// Returns the Current Generation
		unsigned int CurrentGeneration() { return m_Population->current_generation; };

		// Returns an address to the Top Performing Entity
		std::shared_ptr<NeuroEvolution::Entity>& TopEntity() { return m_Population->ReplayEntity; };

		void LoadTopEntity() { m_Population->LoadEntity(); };

		// Check if All Snakes in the population are Dead
		bool isEntitiesDead() { return m_Population->isAllEntitiesDead(); };


	private:
		std::shared_ptr<Population> m_Population;
	};
}

