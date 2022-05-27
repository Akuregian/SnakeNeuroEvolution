#include "../Headers/Wrapper.hpp"

namespace Render
{
	Wrapper::Wrapper()
		: m_Engine(std::make_shared<NeuroEvolution::Engine>()),
		  m_Window(std::make_shared<sf::RenderWindow>()),
		  m_Clock(std::make_shared<sf::Clock>()),
		  _CellSize({ 30, 30 })
	{
		if (!GameSettings::LoadSnake) { m_Engine->CreatePopulation(); }
		if (GameSettings::CreatePopulationOfElites) { ENGINE_LOGGER("Creating Pop Of Elites"); m_Engine->CreatePopulationOfElites(); }

		CreateTextObjects();
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
					if (seg == State[i]->Segments.size() - 1)
					{
						snakeObject->setFillColor(sf::Color(State[i]->colorlist_1[0], State[i]->colorlist_1[1], State[i]->colorlist_1[2], State[i]->brightness));
					}
					else
					{
						snakeObject->setFillColor(sf::Color(State[i]->colorlist_1[0], State[i]->colorlist_1[1], State[i]->colorlist_1[2], State[i]->brightness / 4));
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
		std::shared_ptr<NeuroEvolution::Entity>& Curr_Entity = m_Engine->TopEntity();

		// @@ Temporary @@
		const unsigned int windowOffsetX = 775;
		const unsigned int windowOffsetY = 25;
		const float rate = 0.002f;

		// If its Alive
		if (Curr_Entity->isAlive) {
			float offset_r = (Curr_Entity->colorlist_2[0] - Curr_Entity->colorlist_1[0]);
			float split_r = offset_r / Curr_Entity->Segments.size();
			float temp_r = Curr_Entity->colorlist_1[0];

			float offset_g = (Curr_Entity->colorlist_2[1] - Curr_Entity->colorlist_1[1]);
			float split_g = offset_g / Curr_Entity->Segments.size();
			float temp_g = Curr_Entity->colorlist_1[1];
			
			float offset_b = (Curr_Entity->colorlist_2[2] - Curr_Entity->colorlist_1[2]);
			float split_b = offset_b / Curr_Entity->Segments.size();
			float temp_b = Curr_Entity->colorlist_1[2];

			for (int j = 0; j < Curr_Entity->Segments.size(); j++) {
				// =============== Create the Snake Objects ================
				std::shared_ptr<sf::RectangleShape> EntityObject = std::make_shared<sf::RectangleShape>();
				EntityObject->setSize( _CellSize );
				EntityObject->setPosition(sf::Vector2f((Curr_Entity->Segments[j].first * _CellSize.x) + windowOffsetX - 10, (Curr_Entity->Segments[j].second * _CellSize.y) + windowOffsetY - 10));

				// Interpolate between colorlist_1 -> colorlist_2
				temp_r = (Curr_Entity->colorlist_2[0] - temp_r) * rate + temp_r;
				temp_g = (Curr_Entity->colorlist_2[1] - temp_g) * rate + temp_g;
				temp_b = (Curr_Entity->colorlist_2[2] - temp_b) * rate + temp_b;

				// Change the brightness of the Head
				if (j == Curr_Entity->Segments.size() - 1) 
				{
					EntityObject->setFillColor(sf::Color(temp_r, temp_g, temp_b, Curr_Entity->brightness));
				}
				else 
				{
					EntityObject->setFillColor(sf::Color(temp_r, temp_g, temp_b, (Curr_Entity->brightness / 2)));
				}
				
				// Add to the GameObjects
				GameObjects.push_back(EntityObject);
			}

			// ================ Create the Food Objects ==================
			std::shared_ptr<sf::RectangleShape> FoodObject = std::make_shared<sf::RectangleShape>();
			FoodObject->setSize({ _CellSize.x, _CellSize.y });
			FoodObject->setPosition(sf::Vector2f((Curr_Entity->TargetFood.first * _CellSize.x) + windowOffsetX -  10, (Curr_Entity->TargetFood.second * _CellSize.y) + windowOffsetY - 10));
			FoodObject->setFillColor(sf::Color(0, 255, 0, 255));
			// Add to the GameObjects
			GameObjects.push_back(FoodObject);
		}
	}

	void Wrapper::CreateObjectsForNeuralNetwork()
	{
		if (NeuronObjects.size() == 0 && WeightLines.size() == 0)
		{
			ENGINE_LOGGER("Creating Neural Network Objects");
			// VerticalOffset, HorizontalOffset, XOffset, Spread
			std::vector<unsigned int> Parameters({ 200, 145, 50, 24 });
			sf::Vector2f NeuronSize{ 8, 8 };

			for (size_t j = 0; j < NeuralSettings::TOPOLOGY.size(); j++) {

				// For Every Neuron in each layer
				std::vector<std::shared_ptr<sf::CircleShape>> NeuronsLayer;

				for (uint32_t k = 0; k <NeuralSettings::TOPOLOGY[j]; k++) {
					// Create a Nueron Object
					std::shared_ptr<sf::CircleShape> Neuron = std::make_shared<sf::CircleShape>();
					Neuron->setRadius(8);
					Neuron->setFillColor(sf::Color::White);
					if (j == 0) {
						Neuron->setPosition({ NeuronSize.x + (j * Parameters[1]) + Parameters[2], NeuronSize.y + (k * Parameters[3] + (j * Parameters[0])) });
					}
					else if (j == 1) {
						Neuron->setPosition({ NeuronSize.x + (j * Parameters[1]) + Parameters[2], NeuronSize.y + (k * Parameters[3] + (j * 150)) });
					}
					else if (j == 2) {
						Neuron->setPosition({ NeuronSize.x + (j * Parameters[1]) + Parameters[2], NeuronSize.y + (k * Parameters[3] + (j * 125)) });
					}
					else {
						Neuron->setPosition({ NeuronSize.x + (j * Parameters[1]) + Parameters[2], NeuronSize.y + (k * Parameters[3] + (j * 115)) });
					}

					NeuronsLayer.push_back(Neuron);
				}
				NeuronObjects.push_back(NeuronsLayer);
			}

			unsigned int xOffset = 15;
			unsigned int yOffset = 10;
			for (size_t i = 0; i < NeuronObjects.size(); i++) {
				// For Each Neuron
				std::vector<sf::Vertex> WeightsLayer;
				for (size_t j = 0; j < NeuronObjects[i].size(); j++) {
					if (i != NeuronObjects.size() - 1) {
						const unsigned int& nextLayerSize = NeuronObjects[i + 1].size();
						for (unsigned int line = 0; line < nextLayerSize; line++) {
							WeightsLayer.emplace_back(sf::Vector2f(NeuronObjects[i][j]->getPosition().x + xOffset, NeuronObjects[i][j]->getPosition().y + yOffset), sf::Color::Red);
							WeightsLayer.emplace_back(sf::Vector2f(NeuronObjects[i + 1][line]->getPosition().x + (xOffset - 13), NeuronObjects[i + 1][line]->getPosition().y + yOffset), sf::Color::Red);
						}
					}
				}
				WeightLines.push_back(WeightsLayer);
			}
		}
	}

	void Wrapper::CreateTextObjects()
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			_Text.push_back(std::make_shared<GUI::Text>());
		}
		sf::Vector2f scale(0.5, 0.5);
		_Text[0]->CreateTextObject("UP", sf::Vector2f(520, 350), scale, sf::Color::White);
		_Text[1]->CreateTextObject("DOWN", sf::Vector2f(520, 375), scale, sf::Color::White);
		_Text[2]->CreateTextObject("LEFT", sf::Vector2f(520, 400), scale, sf::Color::White);
		_Text[3]->CreateTextObject("RIGHT", sf::Vector2f(520, 425), scale, sf::Color::White);
	}

	void Wrapper::ReplayAllEntities()
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

	void Wrapper::ReplayBestEntity()
	{
		std::shared_ptr<NeuroEvolution::Entity> Entity = m_Engine->TopEntity();
		SET_SEED(Entity->seed_value);
		while (Entity->isAlive) {
			if (m_Clock->getElapsedTime().asMilliseconds() > GameSettings::TickSpeed)
			{
				m_Clock->restart();
				Entity->Update();
				Wrapper::CreateObjectsForSingleEntity();
				Wrapper::UpdateNetwork();
				Wrapper::DrawObjects();
			}
		}

		m_Engine->TopEntity().reset();
	}

	void Wrapper::UpdateNetwork()
	{
		sf::Color PosWeights = sf::Color(171,212, 193);
		sf::Color NegWeights = sf::Color(126, 133, 139);
		sf::Color ActiveNode = sf::Color(193, 165, 123);
		sf::Color NoneActiveNode = sf::Color::White;

		std::shared_ptr<NeuroEvolution::Entity>& CurrEntity = m_Engine->TopEntity();

		// Update The Neurons
		for (unsigned int i = 0; i < CurrEntity->_Brain->_Neurons.size(); i++) {
			for (unsigned int j = 0; j < CurrEntity->_Brain->_Neurons[i].size(); j++) {
				if (CurrEntity->_Brain->_Neurons[i](j) > 0) {
					NeuronObjects[i][j]->setFillColor(ActiveNode);
				}
				else {
					NeuronObjects[i][j]->setFillColor(NoneActiveNode);
				}

				if (i == CurrEntity->_Brain->_Neurons.size() - 1) {
					Eigen::MatrixXd::Index Max_Coeff_Row, MaxCoeff_Col;
					CurrEntity->_Brain->_Neurons.back().maxCoeff(&Max_Coeff_Row, &MaxCoeff_Col);
					if (j == Max_Coeff_Row) {
						NeuronObjects[i][j]->setFillColor(ActiveNode);
					}
					else {
						NeuronObjects[i][j]->setFillColor(NoneActiveNode);
					}
				}
			}
		}

		// Update The Weights Colors corresponding too Weight Values
		for (int i = 0; i < CurrEntity->_Brain->_Weights.size(); i++) {
			int index = 0;
			for (int j = 0; j < CurrEntity->_Brain->_Weights[i].size(); j++) {
				if (CurrEntity->_Brain->_Weights[i](j) > 0) {
					WeightLines[i][index].color = PosWeights;
					index++;
					WeightLines[i][index].color = PosWeights;
				}
				else {
					WeightLines[i][index].color = NegWeights;
					index++;
					WeightLines[i][index].color = NegWeights;
				}
				index++;
			}
		}
	}

	void Wrapper::DrawObjects()
	{
		m_Window->clear();

		for (auto& i : GameObjects) { m_Window->draw(*i); }

		// Draw Neural Network, Only if were are loading a snake

		for (unsigned int i = 0; i < NeuronObjects.size(); i++) {
			for (unsigned int j = 0; j < NeuronObjects[i].size(); j++)
			{
				m_Window->draw(*NeuronObjects[i][j]);
			}
		}

		// Draw Weights
		for (unsigned int i = 0; i < WeightLines.size(); i++) {
			for (unsigned int j = 0; j < WeightLines[i].size(); j++) {
				if (i == WeightLines.size() - 1 || j == WeightLines[i].size() - 1) {
					continue;
				}
				m_Window->draw(&WeightLines[i][j], 2, sf::Lines);
			}
		}
		
		// Draw Text Objects
		for (unsigned int i = 0; i < _Text.size(); i++)
		{
			m_Window->draw(*_Text[i]->m_text); // TEST
		}

		m_Window->display();
	}

	void Wrapper::Simulate()
	{

		if (GameSettings::DisplayGUI || GameSettings::LoadSnake)
		{
			Wrapper::InitWindow();
			Wrapper::CreateObjectsForNeuralNetwork();
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
			if (GameSettings::DisplayGUI && !GameSettings::LoadSnake)
			{
				m_Engine->TrainPopulation();
				Wrapper::ReplayBestEntity();
			}
			// Display All Snakes
			else if (GameSettings::DisplayGUI && GameSettings::ShowEntirePopulation && !GameSettings::LoadSnake)
			{
				ReplayAllEntities();
			}
			// Load Snake
			else if(GameSettings::LoadSnake && GameSettings::DisplayGUI && !GameSettings::ShowEntirePopulation)
			{
				m_Engine->LoadTopEntity();
				ReplayBestEntity();
			}
		}

		while (m_Engine->CurrentGeneration() < GeneticSettings::MAX_GENERATIONS)
		{
			m_Engine->TrainPopulation();

		}
	}
}
