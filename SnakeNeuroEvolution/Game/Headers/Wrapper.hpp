#pragma once
#include "../../Engine/Headers/Engine.hpp"
#include "SFML/Graphics.hpp"

namespace Render {

	class Wrapper 
	{
	public:
		Wrapper();
		~Wrapper();

		static void Update();
		static void Draw();

	private:
		std::shared_ptr<NeuroEvolution::Engine> m_Engine;
		std::shared_ptr<sf::RenderWindow> m_Window;
	};

}