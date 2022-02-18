#include "../Headers/Engine.hpp"

namespace NeuroEvolution {

	Engine::Engine()
		: _Population(std::make_shared<Population>())
	{
		ENGINE_INIT_WARN("Engine Started");
	}

	Engine::~Engine()
	{
		ENGINE_INIT_WARN("Engine Object Destroyed");
	}

	void Engine::CreatePopulation(const int& pop_size)
	{
		_Population->CreatePopulationOfEntites(pop_size);
		ENGINE_LOGGER_WARN("{0} Entities Added Too The Population", _Population->EntityPopulationSize());
	}

	void Engine::TrainPopulation()
	{
		ENGINE_LOGGER_INFO("Training Population....");
		_Population->TrainEntities();
	}

	void Engine::CreateNextGeneration()
	{
		ENGINE_LOGGER_INFO("Creating Next Population....");
		_Population->CreateNextGeneration();
	}
}

