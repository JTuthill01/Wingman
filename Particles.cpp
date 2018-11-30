#include "Particles.h"

dArr<sf::Texture> Particles::particleTextures;

Particles::Particles(sf::Vector2f position, int textureIndex, sf::Vector2f direction,
	float maxVelocity, float maxRotation, float lifeTime)
{
	this->m_deltaTimeMultiplier = 62.5f;

	this->m_sprite.setTexture(Particles::particleTextures[textureIndex]);
	
	this->m_sprite.setOrigin(this->m_sprite.getGlobalBounds().width / 2,
		this->m_sprite.getGlobalBounds().height / 2);
	
	this->m_sprite.setPosition(position);

	this->m_sprite.setScale(sf::Vector2f(rand() % 2 + 1, rand() % 2 + 1));

	this->m_direction.x = rand() % ((int)direction.x + 10) + ((int)direction.x - 10);
	this->m_direction.y = rand() % ((int)direction.y + 10) + ((int)direction.y - 10);

	this->m_direction = normalize(this->m_direction, vectorLength(this->m_direction));

	this->deceleration = 0.5f;

	this->m_maxVelocity = maxVelocity;

	this->currentVelocity.x = maxVelocity * this->m_direction.x;
	this->currentVelocity.y = maxVelocity * this->m_direction.y; 

	this->m_lifeTime = lifeTime;

	this->m_maxRotation = maxRotation;
}

Particles::~Particles()
{
}

void Particles::update(const float & deltaTime)
{
	//LifeTime
	if (this->m_lifeTime > 0.f)
	{
		this->m_lifeTime -= 1.f * deltaTime * this->m_deltaTimeMultiplier;

		//Reduce Alpha
		if (this->m_sprite.getColor().a > 0) {

			this->m_sprite.setColor(sf::Color(255, 255, 255, this->m_sprite.getColor().a - (rand() % 4 + 1)));

			if (this->m_sprite.getColor().a > 255)
				this->m_sprite.setColor(sf::Color(255, 255, 255, 0));

		}
			
		//Movement
		this->currentVelocity.x += this->deceleration * -this->m_direction.x * deltaTime * this->m_deltaTimeMultiplier;
		this->currentVelocity.y += this->deceleration * -this->m_direction.y * deltaTime * this->m_deltaTimeMultiplier;

		this->m_sprite.move(this->currentVelocity.x * deltaTime * this->m_deltaTimeMultiplier,
			this->currentVelocity.y * deltaTime * this->m_deltaTimeMultiplier);
	
		this->m_sprite.rotate(this->m_maxRotation * deltaTime * this->m_deltaTimeMultiplier);
	}

	
}

void Particles::draw(sf::RenderTarget & target)
{
	target.draw(this->m_sprite);
}

