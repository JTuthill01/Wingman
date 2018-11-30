#include "Pickup.h"

Pickup::Pickup(dArr<sf::Texture> &textures, sf::Vector2f position, int type, float duration)
{
	this->m_deltaTimeMultiplier = 62.5f;

	this->m_textures = &textures;

	this->m_type = type;
	if (this->m_type < (*this->m_textures).size())
		this->m_sprite.setTexture((*this->m_textures)[this->m_type]);

	else
		std::cout << "Error motherfucker no texture exists" << "\n";

	this->m_sprite.setOrigin(this->m_sprite.getGlobalBounds().width / 2,
		this->m_sprite.getGlobalBounds().height / 2);

	this->m_sprite.setPosition(position);
	this->m_sprite.setColor(sf::Color(255, 255, 255, 200));

	this->m_aliveTimerMax = duration;
	this->m_aliveTimer = 0.f;
}

Pickup::~Pickup()
{
}

bool Pickup::checkCollision(sf::FloatRect rect)
{
	if (this->m_sprite.getGlobalBounds().intersects(rect))
		return true;

	return false;
}

void Pickup::update(const float & deltaTime)
{
	if (this->m_aliveTimer < this->m_aliveTimerMax)
		this->m_aliveTimer += 1.f * deltaTime * this->m_deltaTimeMultiplier;

	this->m_sprite.rotate(5.f * deltaTime * this->m_deltaTimeMultiplier);
}

void Pickup::draw(sf::RenderTarget & target)
{
	target.draw(this->m_sprite);
}
