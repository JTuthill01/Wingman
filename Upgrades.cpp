#include "Upgrades.h"

Upgrades::Upgrades(dArr<sf::Texture> &textures, sf::Vector2f position, int type,
	float aliveTimerMax)
{
	this->m_deltaTimeMultiplier = 62.5f;

	this->m_textures = &textures;
	this->m_aliveTimerMax = aliveTimerMax;
	this->m_type = type;
	
	if (this->m_type < (*this->m_textures).size())
		this->m_sprite.setTexture((*this->m_textures)[this->m_type]);

	else
		std::cout << "No Texture was fucking found for that upgrade!!!" << "\n";

	this->m_sprite.setOrigin(this->m_sprite.getGlobalBounds().width / 2, 
		this->m_sprite.getGlobalBounds().height / 2);

	this->m_sprite.setPosition(position);
	this->m_aliveTimer = 0.f;
}

Upgrades::~Upgrades()
{
}

bool Upgrades::checkCollision(const sf::FloatRect rect)
{
	if (this->m_sprite.getGlobalBounds().intersects(rect))
		return true;

	return false;
}

void Upgrades::draw(sf::RenderTarget & target)
{
	target.draw(this->m_sprite);
}

void Upgrades::update(const float deltaTime)
{
	if (this->m_aliveTimer < this->m_aliveTimerMax)
		this->m_aliveTimer += 1.f * deltaTime * this->m_deltaTimeMultiplier;

	this->m_sprite.rotate(5.f * deltaTime * this->m_deltaTimeMultiplier);
}
