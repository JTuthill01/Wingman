#pragma once
#include "Upgrades.h"


class Pickup
{
public:
	Pickup(dArr<sf::Texture> &textures, sf::Vector2f position, int type, float duration);
	virtual ~Pickup();

	inline const int& getType()const { return this->m_type; }
	inline const bool canDelete()const { return this->m_aliveTimer >= this->m_aliveTimerMax; }
	inline const sf::Vector2f& getPosition()const { return this->m_sprite.getPosition(); }

	bool checkCollision(sf::FloatRect rect);

	void update(const float &deltaTime);
	void draw(sf::RenderTarget &target); 

private:
	float m_deltaTimeMultiplier;
	int m_type;

	dArr<sf::Texture> *m_textures;
	sf::Sprite m_sprite;

	float m_aliveTimerMax;
	float m_aliveTimer;
};

