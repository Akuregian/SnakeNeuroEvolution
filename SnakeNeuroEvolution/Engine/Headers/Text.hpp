
#pragma once
#include "SFML/Graphics.hpp"

namespace Text
{

	class Text
	{
	public:
		Text();
		~Text();
		void CreateTextObject(std::string text, sf::Vector2f pos, sf::Vector2f scale, sf::Color col);
		sf::Font m_font;
		std::vector<std::shared_ptr<sf::Text>> textArray;
	};

}
