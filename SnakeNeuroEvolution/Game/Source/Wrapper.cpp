#include "../Headers/Wrapper.hpp"

namespace Render
{
	Wrapper::Wrapper()
		: m_Engine(std::make_shared<NeuroEvolution::Engine>()),
		  m_Window(std::make_shared<sf::RenderWindow>()),
		  m_Clock(std::make_shared<sf::Clock>())
	{

		// CHANGE
		DisplayGUI = false;
		LoadSnake = true;
		DisplayEntireGeneration = false;

		if (!LoadSnake) { m_Engine->CreatePopulation(GeneticSettings::POP_SIZE); }
	}

	Wrapper::~Wrapper()
	{
	}

	void Wrapper::InitWindow()
	{
		m_Window->create(sf::VideoMode(WindowSettings::Width, WindowSettings::Height), "SnakeNeuroEvolution");
	}

	void Wrapper::CreateObjectsForAllEntities(std::vector<std::shared_ptr<NeuroEvolution::Entity>>& State)
	{

		GameObjects.clear();
		sf::Vector2f _CellSize(20, 20);

		for (unsigned int i = 0; i < State.size(); i++)
		{
			int topscore = 0;
			if (State[i]->isAlive)
			{
				for (unsigned int seg = 0; seg < State[i]->Segments.size(); seg++)
				{
					int SnakeScore = State[i]->score;
					std::shared_ptr<sf::RectangleShape> snakeObject = std::make_shared<sf::RectangleShape>();
					snakeObject->setSize(_CellSize);
					snakeObject->setPosition(sf::Vector2f((State[i]->Segments[seg].first * _CellSize.x) + 10, (State[i]->Segments[seg].second * _CellSize.y) + 10));
					if (seg == State[i]->Segments.size())
					{
						if (SnakeScore > topscore)
						{
							topscore = SnakeScore;
							snakeObject->setFillColor(sf::Color(State[i]->colorlist[0], State[i]->colorlist[1], State[i]->colorlist[2], State[i]->brightness));
						}
						else
						{
							snakeObject->setFillColor(sf::Color(State[i]->colorlist[0], State[i]->colorlist[1], State[i]->colorlist[2], State[i]->brightness / 4));
						}
					}
					else
					{
						if (SnakeScore > topscore)
						{
							topscore = SnakeScore;
							snakeObject->setFillColor(sf::Color(State[i]->colorlist[0], State[i]->colorlist[1], State[i]->colorlist[2], State[i]->brightness));
						}
						else
						{
							snakeObject->setFillColor(sf::Color(State[i]->colorlist[0], State[i]->colorlist[1], State[i]->colorlist[2], State[i]->brightness / 4));
						}
					}

					GameObjects.push_back(snakeObject);
					
				}

				std::shared_ptr<sf::RectangleShape> FoodObject = std::make_shared<sf::RectangleShape>();
				FoodObject->setSize({ _CellSize.x, _CellSize.y });
				FoodObject->setPosition(sf::Vector2f((State[i]->TargetFood.first * _CellSize.x) + 10, ((State[i]->TargetFood.second * _CellSize.y)) + 10));
				FoodObject->setFillColor(sf::Color(0, 255, 0, 50));
				GameObjects.push_back(FoodObject);
			}
		}
	}

	void Wrapper::CreateObjectsForSingleEntity()
	{

		GameObjects.clear();
		sf::Vector2f _CellSize(20, 20);
		// If its Alive
		if (m_Engine->TopSnake()->isAlive) {
			for (int j = 0; j < m_Engine->TopSnake()->Segments.size(); j++) {
				// =============== Create the Snake Objects ================
				std::shared_ptr<sf::RectangleShape> snakeObject = std::make_shared<sf::RectangleShape>();
				snakeObject->setSize( _CellSize );
				snakeObject->setPosition(sf::Vector2f((m_Engine->TopSnake()->Segments[j].first * _CellSize.x) + 10, (m_Engine->TopSnake()->Segments[j].second * _CellSize.y + 10)));
				if (j == m_Engine->TopSnake()->Segments.size() - 1) {
					// Change the brightness of the Head
					snakeObject->setFillColor(sf::Color(m_Engine->TopSnake()->colorlist[0], m_Engine->TopSnake()->colorlist[1], m_Engine->TopSnake()->colorlist[2], m_Engine->TopSnake()->brightness));
				}
				else {
					snakeObject->setFillColor(sf::Color(m_Engine->TopSnake()->colorlist[0], m_Engine->TopSnake()->colorlist[1], m_Engine->TopSnake()->colorlist[2], (m_Engine->TopSnake()->brightness / 2)));
				}
				// Add to the GameObjects
				GameObjects.push_back(snakeObject);
			}

			// ================ Create the Food Objects ==================
			std::shared_ptr<sf::RectangleShape> FoodObject = std::make_shared<sf::RectangleShape>();
			FoodObject->setSize({ _CellSize.x, _CellSize.y });
			FoodObject->setPosition(sf::Vector2f((m_Engine->TopSnake()->TargetFood.first * _CellSize.x) + 10, ((m_Engine->TopSnake()->TargetFood.second * _CellSize.y)) + 10));
			FoodObject->setFillColor(sf::Color(0, 255, 0, 255));
			// Add to the GameObjects
			GameObjects.push_back(FoodObject);
		}
	}

	void Wrapper::ReplayAllSnakes()
	{
		std::vector<std::shared_ptr<NeuroEvolution::Entity>> state;
		while (!m_Engine->isEntitiesDead())
		{
			if (m_Clock->getElapsedTime().asMilliseconds() > GameSettings::TickSpeed)
			{
				m_Clock->restart();

				state = m_Engine->TrainOnce();

				CreateObjectsForAllEntities(state);
				DrawObjects();
			}
		}
		m_Engine->CreateNextGeneration();
		state.clear();
	}

	void Wrapper::ReplayBestSnake() {
		SET_SEED(m_Engine->TopSnake()->seed_value);
		while (m_Engine->TopSnake()->isAlive) {
			if (m_Clock->getElapsedTime().asMilliseconds() > GameSettings::TickSpeed) {

				m_Clock->restart();
				m_Engine->TopSnake()->Update();

				Wrapper::CreateObjectsForSingleEntity();
				Wrapper::DrawObjects();
			}
		}
	}

	void Wrapper::DrawObjects()
	{
		m_Window->clear();

		for (auto& i : GameObjects)
		{
			m_Window->draw(*i);
		}

		m_Window->display();
	}

	void Wrapper::Simulate()
	{

		if (DisplayGUI || LoadSnake)
		{
			Wrapper::InitWindow();
		}

		while (m_Window->isOpen()) 
		{
			sf::Event events;
			while (m_Window->pollEvent(events))
			{
				// Close window: exit
				if (events.type == sf::Event::Closed) {
					m_Window->close();
				}
			}

			// Display Top Snake
			if (!DisplayEntireGeneration && !LoadSnake)
			{
				m_Engine->TrainPopulation();
				Wrapper::ReplayBestSnake();

			}
			// Display All Snakes
			else if (DisplayEntireGeneration && !LoadSnake)
			{
				ReplayAllSnakes();
			}
			// Load Snake
			else
			{
				m_Engine->LoadTopEntity();
				ReplayBestSnake();
			}
		}

		while (m_Engine->CurrentGeneration() < GeneticSettings::MAX_GENERATIONS)
		{
			m_Engine->TrainPopulation();
		}
	}
}
