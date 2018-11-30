#pragma once
#include "Pickup.h"

class TextTag
{
public:
	TextTag(sf::Font* font, std::string text, sf::Color color, 
		sf::Vector2f position, sf::Vector2f direction, 
		unsigned int size, float timerMax, bool accelerate);
	virtual ~TextTag();

	//Accessors
	inline const float& getTimer()const { return this->m_timer; }

	void update(const float &deltaTime);
	void draw(sf::RenderTarget &target);


private:
	sf::Font* m_font;
	sf::Text m_text; 

	//Timers
	float m_timerMax;
	float m_timer; 

	bool m_follow; 
	bool m_accerlate; 

	float m_speed; 
	sf::Vector2f m_direction; 

	float m_deltaTimeMultiplier; 
};

