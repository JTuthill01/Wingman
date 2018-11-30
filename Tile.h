#pragma once
#include "Particles.h"

class Tile
{
public:
	Tile(sf::IntRect textureRect, sf::Vector2f position, bool isCollider, bool isDamaging);
	virtual ~Tile();

	//Static Textures
	static sf::Texture tileTextures;

	//Inline Functions
	inline const sf::Vector2f getPosition()const { return this->m_sprite.getPosition(); }
	inline const sf::FloatRect getGobalBounds()const { return this->m_sprite.getGlobalBounds(); }
	inline const bool getIsDamging()const { return this->m_isDamaging; }
	inline const bool getIsCollider()const { return this->m_isCollider; }
	inline void setPosition(float x, float y) { this->m_sprite.setPosition(sf::Vector2f(x, y)); }
	inline void setTextureRect(int x, int y, int width, int height) { this->m_sprite.setTextureRect(sf::IntRect(x, y, width, height)); }
	inline void setIsCollider(bool isCollider) { this->m_isCollider = isCollider; }
	inline void setIsDamaging(bool isDamaging) { this->m_isDamaging = isDamaging; }

	//Functions
	void update(const float &deltaTime);
	void draw(sf::RenderTarget &target);

private:
	//Core
	bool m_isCollider;
	bool m_isDamaging;

	int m_damage;
	float m_deltaTimeMultiplier;


	//Positioning

	//Visual
	sf::Sprite m_sprite; 
};

