#include "Bullet.h"

Bullet::Bullet(sf::Texture* texture, sf::Vector2f position, sf::Vector2f scale, sf::Vector2f direction,
	float initalVelocity, float maxVelocity, float acceleration)
{
	this->m_deltaTimeMultiplier = 62.5f;

	this->m_texture = texture; 
	this->m_sprite.setTexture(*this->m_texture);

	this->m_maxVelocity = maxVelocity;

	this->m_direction = direction;

	this->m_acceleration = acceleration;

	this->m_initalVelocity = initalVelocity; 

	this->m_currentVelocity = sf::Vector2f(initalVelocity * this->m_direction.x, 
		initalVelocity * this->m_direction.y);
	
	this->m_sprite.setOrigin(this->m_sprite.getGlobalBounds().width / 2,
		this->m_sprite.getGlobalBounds().height / 2);

	this->m_sprite.setScale(scale);
	this->m_sprite.setPosition(position);
	this->m_sprite.setRotation(atan2(this->m_direction.y, this->m_direction.x) 
		* 180 / thor::Pi + 180);
}

Bullet::~Bullet()
{
}

void Bullet::movement(const float &deltaTime)
{
	if (this->m_acceleration > 0.0f) {

		if (this->m_currentVelocity.x < this->m_maxVelocity)
			this->m_currentVelocity.x += this->m_acceleration * this->m_direction.x
			* deltaTime * this->m_deltaTimeMultiplier;

		if (this->m_currentVelocity.y < this->m_maxVelocity)
			this->m_currentVelocity.y += this->m_acceleration * this->m_direction.y
			* deltaTime * this->m_deltaTimeMultiplier;
	}
	else
	{
		this->m_currentVelocity = sf::Vector2f(this->m_maxVelocity * this->m_direction.x, this->m_maxVelocity * this->m_direction.y);
	}

	this->m_sprite.move(this->m_currentVelocity * deltaTime * this->m_deltaTimeMultiplier);
}

void Bullet::update(const float &deltaTime)
{
	this->movement(deltaTime);
}

void Bullet::draw(sf::RenderTarget & target)
{
	target.draw(this->m_sprite);
}
