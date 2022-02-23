#include "../Headers/Engine.hpp"

namespace NeuroEvolution {

	Engine::Engine()
		: m_Population(std::make_shared<Population>())

	{
		ENGINE_INIT_WARN("Engine Started");
		Seed::Init();
	}

	Engine::~Engine()
	{
		ENGINE_INIT_ERROR("Engine Object Destroyed");
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
}

