#include "../Headers/Wrapper.hpp"

namespace Render
{
	Wrapper::Wrapper()
		: m_Engine(std::make_shared<NeuroEvolution::Engine>()),
		  m_Window(std::make_shared<sf::RenderWindow>())
	{
		ENGINE_INIT_WARN("Wrapper Instanstiated");
		m_Window->create(sf::VideoMode(Window::Width, Window::Height), "SnakeNeuroEvolution");
		m_Engine->CreatePopulation(GeneticSettings::POP_SIZE);
	}

	Wrapper::~Wrapper()
	{
		ENGINE_INIT_WARN("Wrapper Object Destroyed");
	}
}