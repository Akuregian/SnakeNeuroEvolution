#include "../../Engine/Headers/GUI.hpp"

namespace GUI
{

	// ================ Text Class ===================
	Text::Text()
		: m_text(std::make_shared<sf::Text>())
	{
		if (!m_font.loadFromFile("../../../../SnakeNeuroEvolution/Engine/Font/arial.ttf"))
		{
			std::cout << "Error in Loading Font\n";
		}
	}

	Text::~Text()
	{
	}

	void Text::CreateTextObject(std::string text, sf::Vector2f pos, sf::Vector2f scale, sf::Color col, sf::Uint32 style)
	{
		m_text->setFont(m_font);
		m_text->setString(text);
		m_text->setPosition(pos);
		m_text->setScale(scale);
		m_text->setFillColor(col);
		m_text->setStyle(style);
	}

	void Text::SetString(std::string text) {
		m_text->setString(text);
	}
}
