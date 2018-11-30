#include "Tile.h"

sf::Texture Tile::tileTextures;

Tile::Tile(sf::IntRect textureRect, sf::Vector2f position, bool isCollider, bool isDamaging)
{
	this->m_deltaTimeMultiplier = 62.5f;

	this->m_sprite.setTexture(Tile::tileTextures);
	this->m_sprite.setTextureRect(textureRect);
	this->m_sprite.setPosition(position);

	this->m_isCollider = isCollider;
	this->m_isDamaging = isDamaging; 
}

Tile::~Tile()
{
}

void Tile::update(const float & deltaTime)
{
}

void Tile::draw(sf::RenderTarget & target)
{
	target.draw(this->m_sprite);
}
