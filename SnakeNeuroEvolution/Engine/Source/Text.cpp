#include "../../Engine/Headers/Text.hpp"

namespace Text
{
	Text::Text()
	{
		if (!m_font.loadFromFile("../../../../SnakeNeuroEvolution/Engine/Font/arial.ttf"))
		{
			std::cout << "Error in Loading Font\n";
		}
	}

	Text::~Text()
	{
	}
	
	void Text::CreateTextObject(std::string text, sf::Vector2f pos, sf::Vector2f scale, sf::Color col)
	{
		std::shared_ptr<sf::Text> Text = std::make_shared<sf::Text>();
		Text->setFont(m_font);
		Text->setString(text);
		Text->setPosition(pos);
		Text->setScale(scale);
		Text->setFillColor(col);
		textArray.push_back(Text);
	}
}
