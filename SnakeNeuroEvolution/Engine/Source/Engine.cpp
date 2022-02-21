#include "../Headers/Engine.hpp"

namespace NeuroEvolution {

	Engine::Engine()
		: m_Population(std::make_shared<Population>()),
		  m_Seed(std::make_shared<Seed>())

	{
		ENGINE_INIT_WARN("Engine Started");
	}

	Engine::~Engine()
	{
		ENGINE_INIT_WARN("Engine Object Destroyed");
	}

	void Engine::CreatePopulation(const int& pop_size)
	{
		m_Population->CreatePopulationOfEntites(pop_size);
		ENGINE_LOGGER_WARN("{0} Entities Added Too The Population", m_Population->EntityPopulationSize());
	}

	void Engine::TrainPopulation()
	{
		ENGINE_LOGGER_INFO("Training Population....");
		m_Population->TrainEntities();
	}

	void Engine::CreateNextGeneration()
	{
		ENGINE_LOGGER_INFO("Creating Next Population....");
		m_Population->CreateNextGeneration();
	}
}

