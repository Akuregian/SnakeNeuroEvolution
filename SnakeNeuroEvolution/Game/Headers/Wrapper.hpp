#pragma once
#include "../../Engine/Headers/Engine.hpp"
#include "SFML/Graphics.hpp"

namespace Render {

	class Wrapper 
	{
	public:
		Wrapper();
		~Wrapper();

		void InitWindow();
		void Simulate();
		void CreateObjectsForAllEntities(std::vector<std::shared_ptr<NeuroEvolution::Entity>>& State);
		void CreateObjectsForSingleEntity();
		void ReplayAllSnakes();
		void ReplayBestSnake();
		void DrawObjects();

	private:
		std::shared_ptr<NeuroEvolution::Engine> m_Engine;
		std::shared_ptr<sf::RenderWindow> m_Window;
		std::shared_ptr<sf::Clock> m_Clock;

		std::vector<std::shared_ptr<sf::RectangleShape>> GameObjects;

		bool ShowGame;
		bool LoadSnake;
		bool DisplayEntireGeneration;
	};

}