#pragma once
#include "TextTag.h"

class Bullet
{
public:
	Bullet(sf::Texture* texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f direction, 
		float initalVelocity, float maxVelocity, float acceleration);
	virtual ~Bullet();

	//Accessors
	inline sf::FloatRect getGobalBounds()const { return this->m_sprite.getGlobalBounds(); }
	inline const sf::Vector2f& getPosition()const { return this->m_sprite.getPosition(); }
	inline sf::Vector2f getNormalDirection() { return normalize(this->m_currentVelocity, vectorLength(this->m_currentVelocity)); }
	inline sf::Vector2f getVelocity()const { return this->m_currentVelocity; }

	//Functions
	void movement(const float &deltaTime);
	void update(const float &deltaTime);
	void draw(sf::RenderTarget &target);

	//Direction Functions

	float vectorLength(sf::Vector2f v)
	{
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}

	sf::Vector2f normalize(sf::Vector2f v, float length)
	{
		if (length == 0)
			return sf::Vector2f(0.f, 0.f);

		else
			return v / length;
	}

private:
	sf::Texture* m_texture;
	sf::Sprite m_sprite; 

	sf::Vector2f m_currentVelocity; 
	float m_initalVelocity; 
	float m_maxVelocity;
	float m_acceleration; 
	sf::Vector2f m_direction;

	float m_deltaTimeMultiplier; 
};

