#include "../Headers/Wrapper.hpp"

namespace Render
{
	Wrapper::Wrapper()
		: m_Engine(std::make_shared<NeuroEvolution::Engine>()),
			m_Window(std::make_shared<sf::RenderWindow>()),
			m_Clock(std::make_shared<sf::Clock>()),
			_CellSize({ 30, 30 })
	{

		// CHANGE
		DisplayGUI = true;
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
						snakeObject->setFillColor(sf::Color(State[i]->colorlist[0], State[i]->colorlist[1], State[i]->colorlist[2], State[i]->brightness));
					}
					else
					{
						snakeObject->setFillColor(sf::Color(State[i]->colorlist[0], State[i]->colorlist[1], State[i]->colorlist[2], State[i]->brightness / 4));
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
		const unsigned int windowOffsetX = 600;
		const unsigned int windowOffsetY = 50;

		// If its Alive
		if (Curr_Entity->isAlive) {
			for (int j = 0; j < Curr_Entity->Segments.size(); j++) {
				// =============== Create the Snake Objects ================
				std::shared_ptr<sf::RectangleShape> EntityObject = std::make_shared<sf::RectangleShape>();
				EntityObject->setSize( _CellSize );
				EntityObject->setPosition(sf::Vector2f((Curr_Entity->Segments[j].first * _CellSize.x) + windowOffsetX - 10, (Curr_Entity->Segments[j].second * _CellSize.y) + windowOffsetY - 10));
				if (j == Curr_Entity->Segments.size() - 1) {
					// Change the brightness of the Head
					EntityObject->setFillColor(sf::Color(Curr_Entity->colorlist[0], Curr_Entity->colorlist[1], Curr_Entity->colorlist[2], Curr_Entity->brightness));
				}
				else {
					EntityObject->setFillColor(sf::Color(Curr_Entity->colorlist[0], Curr_Entity->colorlist[1], Curr_Entity->colorlist[2], (Curr_Entity->brightness / 2)));
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
		ENGINE_LOGGER("Creating Neural Network Objects");
		// VerticalOffset, HorizontalOffset, XOffset, Spread
		std::vector<unsigned int> Parameters({ 200, 145, 50, 24 });
		std::shared_ptr<NeuroEvolution::Entity>& CurrEntity = m_Engine->TopEntity();
		sf::Vector2f NeuronSize{ 8, 8 };

		for (size_t j = 0; j < CurrEntity->_Brain->_Neurons.size(); j++) {

			// For Every Neuron in each layer
			std::vector<std::shared_ptr<sf::CircleShape>> NeuronsLayer;

			for (uint32_t k = 0; k < CurrEntity->_Brain->_Neurons[j].size(); k++) {
				// Create a Nueron Object
				std::shared_ptr<sf::CircleShape> Neuron = std::make_shared<sf::CircleShape>();
				Neuron->setRadius(8);
				Neuron->setFillColor(sf::Color::White);
				if (j == 0) {
					Neuron->setPosition({ NeuronSize.x + (j * Parameters[1]) + Parameters[2], NeuronSize.y + (k * Parameters[3] + (j * Parameters[0]))});
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

	void Wrapper::ReplayBestEntity() {
		SET_SEED(m_Engine->TopEntity()->seed_value);

		Wrapper::CreateObjectsForNeuralNetwork();

		while (m_Engine->TopEntity()->isAlive) {
			if (m_Clock->getElapsedTime().asMilliseconds() > GameSettings::TickSpeed) {

				m_Clock->restart();
				m_Engine->TopEntity()->Update();

				Wrapper::CreateObjectsForSingleEntity();
				Wrapper::UpdateNetwork();
				Wrapper::DrawObjects();
			}
		}
	}

	void Wrapper::UpdateNetwork()
	{

		std::shared_ptr<NeuroEvolution::Entity>& CurrEntity = m_Engine->TopEntity();

		// Update The Neurons
		for (unsigned int i = 0; i < CurrEntity->_Brain->_Neurons.size(); i++) {
			for (unsigned int j = 0; j < CurrEntity->_Brain->_Neurons[i].size(); j++) {
				if (CurrEntity->_Brain->_Neurons[i](j) > 0) {
					NeuronObjects[i][j]->setFillColor(sf::Color(31, 198, 0));
				}
				else {
					NeuronObjects[i][j]->setFillColor(sf::Color::White);
				}

				if (i == CurrEntity->_Brain->_Neurons.size() - 1) {
					Eigen::MatrixXd::Index Max_Coeff_Row, MaxCoeff_Col;
					CurrEntity->_Brain->_Neurons.back().maxCoeff(&Max_Coeff_Row, &MaxCoeff_Col);
					if (j == Max_Coeff_Row) {
						NeuronObjects[i][j]->setFillColor(sf::Color(14, 255, 0));
					}
					else {
						NeuronObjects[i][j]->setFillColor(sf::Color::White);
					}
				}
			}
		}

		// Update The Weights Colors corresponding too Weight Values
		for (int i = 0; i < CurrEntity->_Brain->_Weights.size(); i++) {
			int index = 0;
			for (int j = 0; j < CurrEntity->_Brain->_Weights[i].size(); j++) {
				if (CurrEntity->_Brain->_Weights[i](j) > 0) {
					WeightLines[i][index].color = sf::Color::Blue;
					index++;
					WeightLines[i][index].color = sf::Color::Blue;
				}
				else {
					WeightLines[i][index].color = sf::Color::Red;
					index++;
					WeightLines[i][index].color = sf::Color::Red;
				}
				index++;
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

		// Draw Neural Network, Only if were are loading a snake

		for (unsigned int i = 0; i < NeuronObjects.size(); i++)
		{
			for (unsigned int j = 0; j < NeuronObjects[i].size(); j++)
			{
				m_Window->draw(*NeuronObjects[i][j]);
			}
		}


		// Draw Weights
		for (int i = 0; i < WeightLines.size(); i++) {
			for (int j = 0; j < WeightLines[i].size(); j++) {
				if (i == WeightLines.size() - 1 || j == WeightLines[i].size() - 1) {
					continue;
				}
				m_Window->draw(&WeightLines[i][j], 2, sf::Lines);
			}
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
				Wrapper::ReplayBestEntity();

			}
			// Display All Snakes
			else if (DisplayEntireGeneration && !LoadSnake)
			{
				ReplayAllEntities();
			}
			// Load Snake
			else
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
