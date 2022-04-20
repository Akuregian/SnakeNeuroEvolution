#include "../Headers/Population.hpp"


namespace NeuroEvolution {

	Population::Population()
		: current_generation(0)
	{
		ENGINE_INIT_WARN("Population Class Initialized");
	}

	Population::~Population()
	{
		ENGINE_INIT_ERROR("Population Class Destroyed");
	}

	void Population::CreatePopulationOfEntites(const int& pop_size)
	{
		for (unsigned int i = 0; i < pop_size; i++)
		{
			_EntityPopulation.push_back(std::make_shared<NeuroEvolution::Entity>(NeuralSettings::TOPOLOGY));
		}
	}

	bool Population::isAllSnakesDead()
	{
		for (unsigned int i = 0; i < _EntityPopulation.size(); i++)
		{
			if (_EntityPopulation[i]->isAlive)
			{
				return false;
			}
		}
		return true;
	}

	void Population::Update()
	{
		for (unsigned int i = 0; i < _EntityPopulation.size(); i++)
		{
			_EntityPopulation[i]->Update();
		}
	}

	void Population::TrainEntities()
	{
		while (!isAllSnakesDead())
		{
			Update();
		}

		Population::CreateNextGeneration();
	}

	void Population::CreateNextGeneration()
	{

		// Calculate Each Snakes Fitness Score and set it too Network Fitness
		for (unsigned int i = 0; i < _EntityPopulation.size(); i++) 
		{
			double FitnessScore = GeneticAlgorithm::CalculateFitness(_EntityPopulation[i]->steps, _EntityPopulation[i]->score);
			_EntityPopulation[i]->_Brain->NetworkFitness = FitnessScore;
		}

		// Clear MatingPool
		_EntityMatingPool.clear();

		// Sort 'Elites' by fitness
		GeneticAlgorithm::ElitismSelection(_EntityPopulation);

		// Grabbing Roulette Wheel Sum
		double roulette_wheel_sum = 0;
		for (unsigned int i = 0; i < _EntityPopulation.size(); i++)
		{
			roulette_wheel_sum += _EntityPopulation[i]->_Brain->NetworkFitness;
		}

		// Print Results and Set the top performing Snake
		Population::Results();

		// Store 'Elites' into MatingPool
		for (unsigned int i = 0; i < GeneticSettings::POP_SIZE; i++)
		{
			_EntityMatingPool.push_back(std::make_shared<Entity>(_EntityPopulation[i]->_Brain->_Weights, _EntityPopulation[i]->_Brain->_Bias, NULL));
			_EntityMatingPool[i]->_Brain->NetworkFitness = _EntityPopulation[i]->_Brain->NetworkFitness;
		}

		_EntityPopulation = _EntityMatingPool;

		while (_EntityPopulation.size() < GeneticSettings::MATING_POP_SIZE)
		{
			GeneticAlgorithm::CrossoverAndMutation(roulette_wheel_sum, _EntityMatingPool, _EntityPopulation);
		}

		for (int i = 0; i < 250; i++)
		{
			_EntityPopulation[i]->_Brain->NetworkFitness = 0;
			_EntityPopulation[i]->score = 0;
		}

		ENGINE_LOGGER_INFO("Crossover and Mutation Completed, Current Entity PopSize: {0}", _EntityPopulation.size());

		current_generation++;
	}

	void Population::Results()
	{
		//--------------------- Print To Console ------------------------
		ENGINE_RESULTS_LOGGER("Generation: {0}", current_generation);
		ENGINE_RESULTS_LOGGER("--------------------------------------------------------");
		for (unsigned i = 0; i < 1; i++) {

			ENGINE_RESULTS_LOGGER("Score: {0}", _EntityPopulation[i]->score);
			ENGINE_RESULTS_LOGGER("Network Fitness: {0}", _EntityPopulation[i]->_Brain->NetworkFitness);
		}
		ENGINE_RESULTS_LOGGER("--------------------------------------------------------\n\n");

		//--------------------------------------------------------------
		
		
		// Top Snake
		ENGINE_RESULTS_LOGGER("TopSeed: {0}", _EntityPopulation.front()->seed_value);
 		ReplaySnake = std::make_shared<Entity>(_EntityPopulation.front()->_Brain->_Weights, _EntityPopulation.front()->_Brain->_Bias, _EntityPopulation.front()->seed_value);
	}
}