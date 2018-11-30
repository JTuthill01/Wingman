#pragma once
#include "Tile.h"

class Upgrades
{
public:
	Upgrades(dArr<sf::Texture> &textures, sf::Vector2f position, int type,
		float aliveTimerMax);
	virtual ~Upgrades();

	inline const int& getType() { return this->m_type; }
	inline bool canDelete() { return this->m_aliveTimer >= this->m_aliveTimerMax; }

	bool checkCollision(const sf::FloatRect rect);
	void draw(sf::RenderTarget &target);
	void update(const float deltaTime);

private:
	int m_type;
	sf::Vector2f m_position;

	float m_aliveTimerMax;
	float m_aliveTimer; 

	dArr<sf::Texture>* m_textures;
	sf::Sprite m_sprite;

	float m_deltaTimeMultiplier; 
};

