#include "../Headers/Population.hpp"


namespace NeuroEvolution {

	Population::Population()
	{
		ENGINE_INIT_WARN("Population Class Initialized");
	}

	Population::~Population()
	{
		ENGINE_INIT_WARN("Population Class Destroyed");
	}

	void Population::CreatePopulationOfEntites(const int& pop_size) 
	{
		
		for (unsigned int i = 0; i < pop_size; i++)
		{
			_EntityPopulation.push_back(std::make_shared<NeuroEvolution::Entity>(NeuralSettings::TOPOLOGY));
		}
	}

	void Population::TrainEntities()
	{
		for (unsigned int i = 0; i < _EntityPopulation.size(); i++)
		{
			_EntityPopulation[i]->UpdateVisionVector();
			_EntityPopulation[i]->direction = _EntityPopulation[i]->_Brain->Train(_EntityPopulation[i]->_Brain, _EntityPopulation[i]->input_vector);
		}
		ENGINE_LOGGER_INFO("All Entities Trained");
	}

	void Population::CreateNextGeneration()
	{
		ENGINE_LOGGER_INFO("Calculating Each Entity's Fitness");
		for (unsigned int i = 0; i < _EntityPopulation.size(); i++) 
		{
			double FitnessScore = GeneticAlgorithm::CalculateFitness(_EntityPopulation[i]->steps, _EntityPopulation[i]->score);
			_EntityPopulation[i]->_Brain->NetworkFitness = FitnessScore;
		}

		// Sort 'Elites' by fitness
		GeneticAlgorithm::ElitismSelection(_EntityPopulation);

		// Store 'Elites' into MatingPool
		for (unsigned int i = 0; i < GeneticSettings::MATING_POP_SIZE; i++)
		{
			_EntityMatingPool.push_back(_EntityPopulation[i]);
		}
		
		// Clear Population of Stored Pointers, Effectivly Releasing the memory (b/c there smart pointers)
		_EntityPopulation.clear();

		ENGINE_LOGGER_INFO("Add {0} Elites To MatingPool", _EntityMatingPool.size());

		// Grabbing Roulette Wheel Sum
		double roulette_wheel_sum = 0;
		for (unsigned int i = 0; i < _EntityMatingPool.size(); i++)
		{
			roulette_wheel_sum += _EntityMatingPool[i]->_Brain->NetworkFitness;
		}

		ENGINE_LOGGER_INFO("Creating Remaining Children using Top Fitness Scores");

		_EntityPopulation = _EntityMatingPool;

		while (_EntityPopulation.size() < GeneticSettings::POP_SIZE)
		{
			GeneticAlgorithm::CrossoverAndMutation(roulette_wheel_sum, _EntityMatingPool, _EntityPopulation);
		}
	}
}