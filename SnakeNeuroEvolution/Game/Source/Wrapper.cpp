#include "../Headers/Wrapper.hpp"

namespace Render
{
	Wrapper::Wrapper()
		: m_Engine(std::make_shared<NeuroEvolution::Engine>()),
		  m_Window(std::make_shared<sf::RenderWindow>()),
		  m_Clock(std::make_shared<sf::Clock>())
	{
		m_Engine->CreatePopulation(GeneticSettings::POP_SIZE);

		// CHANGE
		ShowGame = true;
		LoadSnake = false;
	}

	Wrapper::~Wrapper()
	{
	}

	void Wrapper::InitWindow()
	{
		m_Window->create(sf::VideoMode(WindowSettings::Width, WindowSettings::Height), "SnakeNeuroEvolution");
	}

	void Wrapper::CreateObjects()
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
				snakeObject->setOutlineThickness(1);
				snakeObject->setOutlineColor(sf::Color::White);
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
			FoodObject->setOutlineThickness(1);
			FoodObject->setOutlineColor(sf::Color::White);
			FoodObject->setFillColor(sf::Color(0, 255, 0, 255));
			// Add to the GameObjects
			GameObjects.push_back(FoodObject);
		}
	}


	void Wrapper::ReplayBestSnake() {
	//	if (TopScore > PreviousScore || LoadSnake) {
	//		PreviousScore = TopScore;

		SET_SEED(m_Engine->TopSnake()->seed_value);
		
			while (m_Engine->TopSnake()->isAlive) {
				if (m_Clock->getElapsedTime().asMilliseconds() > GameSettings::TickSpeed) {

					m_Clock->restart();

					m_Engine->TopSnake()->Update();

				//	if (Replay_Snake->score > TopScore) {
				//		TopScore = Replay_Snake->score;
				//	}

				//	if (Replay_Snake->Segments.size() == (Settings::COLS * Settings::ROWS)) {
				//		std::cout << "Snake Died or Completed Game" << std::endl;
				//		std::cin.get();
				//	}
				//	Update();
				//	Draw();
					Wrapper::CreateObjects();
					Wrapper::DrawObjects();
				}
			}

	//	}
//		else {
//			m_win.clear();
//			ShowText();
//			m_win.draw(Training_Text);
//			DrawNeuralNetwork();
//			m_win.display();
//		}
//		delete Replay_Snake;
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

		if (ShowGame || LoadSnake)
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

			if (!LoadSnake)
			{
				m_Engine->TrainPopulation();
				//Wrapper::ReplayBestSnake();

			}
			else
			{
			//	m_Engine->LoadSnake();
			//	m_Engine->ReplayTopSnake();
			}

		}

		while (m_Engine->CurrentGeneration() < GeneticSettings::MAX_GENERATIONS)
		{
			m_Engine->TrainPopulation();
		}
	}
}
