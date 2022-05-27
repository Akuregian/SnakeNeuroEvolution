#pragma once
#include "../../Engine/Headers/Engine.hpp"
#include "SFML/Graphics.hpp"
#include "../../Engine/Headers/Text.hpp"


namespace Render {

	typedef std::vector<std::vector<std::shared_ptr<sf::CircleShape>>> TWO_DIM_C;
	typedef std::vector<std::vector<sf::Vertex>> TWO_DIM_V;

	class Wrapper 
	{
	public:
		Wrapper();
		~Wrapper();

		void InitWindow();
		void Simulate();

		// Create Objects
		void CreateObjectsForAllEntities(std::vector<std::shared_ptr<NeuroEvolution::Entity>>& State);
		void CreateObjectsForSingleEntity();
		void CreateObjectsForNeuralNetwork();
		void CreateTextObjects();

		// Replay
		void ReplayAllEntities();
		void ReplayBestEntity();

		// Draw
		void UpdateNetwork();
		void DrawObjects();

	private:
		std::shared_ptr<NeuroEvolution::Engine> m_Engine;
		std::shared_ptr<sf::RenderWindow> m_Window;
		std::shared_ptr<sf::Clock> m_Clock;
		std::vector<std::shared_ptr<sf::RectangleShape>> GameObjects;
		TWO_DIM_C NeuronObjects;
		TWO_DIM_V WeightLines;
		sf::Vector2f _CellSize;

		std::shared_ptr<Text::Text> _Text;
	};
}