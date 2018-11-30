#include "TextTag.h"

TextTag::TextTag(sf::Font* font, std::string text, sf::Color color, 
	sf::Vector2f position, sf::Vector2f direction,
	unsigned int size, float timerMax, bool accerlate)
{
	this->m_deltaTimeMultiplier = 62.5f;

	this->m_text.setFont(*font);
	this->m_text.setCharacterSize(size);
	this->m_text.setPosition(position);
	this->m_text.setString(text);
	this->m_text.setFillColor(color);

	this->m_speed = 1.2f;
	this->m_direction.x = direction.x;
	this->m_direction.y = direction.y;
	this->m_timerMax = timerMax;
	this->m_timer = this->m_timerMax;
	this->m_accerlate = accerlate;
}

TextTag::~TextTag()
{
}

void TextTag::update(const float & deltaTime)
{
	//Update Timer
	if (this->m_timer > 0.f)
	{
		this->m_timer -= 1.f * deltaTime * this->m_deltaTimeMultiplier;

		if (this->m_accerlate)
		{
			if (this->m_timer > this->m_timerMax / 2)
			{
				this->m_text.move(this->m_direction.x * (this->m_speed * 5) * deltaTime * this->m_deltaTimeMultiplier * 2.f,
					this->m_direction.y * (this->m_speed * 5) * deltaTime * this->m_deltaTimeMultiplier * 2.f);
			}
			else
			{
				this->m_text.move(this->m_direction.x * this->m_speed * deltaTime * this->m_deltaTimeMultiplier,
					this->m_direction.y * this->m_speed * deltaTime * this->m_deltaTimeMultiplier);
			}
		}
		else
		{
			this->m_text.move(this->m_direction.x * this->m_speed * deltaTime * this->m_deltaTimeMultiplier,
				this->m_direction.y * this->m_speed * deltaTime * this->m_deltaTimeMultiplier);
		}
	}
	else
		this->m_text.setFillColor(sf::Color(0, 0, 0, 255));
	
}

void TextTag::draw(sf::RenderTarget &target)
{
	target.draw(this->m_text);
}
