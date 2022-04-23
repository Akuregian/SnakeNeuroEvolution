#include "../Headers/Engine.hpp"

namespace NeuroEvolution {

	Engine::Engine()
		: m_Population(std::make_shared<Population>())

	{
		Seed::Init();
	}

	Engine::~Engine()
	{
	}

	void Engine::CreatePopulation(const int& pop_size)
	{
		m_Population->CreatePopulationOfEntites(pop_size);
	}

	std::vector<std::shared_ptr<NeuroEvolution::Entity>>& Engine::TrainOnce()
	{
		m_Population->TrainEntitiesOnce();
		return m_Population->_EntityPopulation;
	}

	void Engine::TrainPopulation()
	{
		m_Population->TrainEntities();
	}

}

