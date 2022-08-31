
#pragma once
#include "SFML/Graphics.hpp"

namespace GUI
{

	class Text 
	{
	public:
		Text();
		~Text();
		void LoadFont();
		void CreateTextObject(std::string text, sf::Vector2f pos, sf::Vector2f scale, sf::Color col, sf::Uint32 style);
		void SetString(std::string text);
		sf::Font m_font;
		std::shared_ptr<sf::Text> m_text;
	};
}
