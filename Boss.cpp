#include "Boss.h"

enum textures { BODY = 0 };

//Texture Rule: 0 for body, 1 for regular gun, 2 for regular bullets

Boss::Boss(dArr<sf::Texture> &bodyTextures, dArr<sf::Texture> &gunTextures, dArr<sf::Texture> &bulletTextures, sf::Vector2f position, int type)
{
	this->m_bodyTextures = &bodyTextures;
	this->m_gunTextures = &gunTextures;
	this->m_bulletTextures = &bulletTextures;
	this->m_type = type;
	
	this->m_deltaTimeMultiplier = 62.5f;

	switch (this->m_type)
	{
	case 0: //First Boss (Easy???)

		this->m_sprite.setTexture(sf::Texture((*this->m_bodyTextures)[0]));

		this->m_guns.add(BossGun(&(*this->m_gunTextures)[0], sf::Vector2f(0.f, 0.f), 16.f));
		this->m_guns.add(BossGun(&(*this->m_gunTextures)[0], sf::Vector2f(0.f, 0.f), 16.f));
		this->m_guns.add(BossGun(&(*this->m_gunTextures)[0], sf::Vector2f(0.f, 0.f), 16.f));


		this->m_sprite.setPosition(position);

		break;

	case 1:
		break;

	default:
		break;
	}
}

Boss::~Boss()
{
}

void Boss::movement()
{

}

void Boss::update(const float &deltaTime)
{
	for (size_t j = 0; j < this->m_guns.size(); j++)
	{
		this->m_guns[j].update(deltaTime);
	}

	for (size_t i = 0; i < this->m_bullets.size(); i++)
	{
		this->m_bullets[i].update(deltaTime);
	}
}

void Boss::draw(sf::RenderTarget & target)
{
	target.draw(this->m_sprite);

	for (size_t i = 0; i < this->m_guns.size(); i++)
	{
		this->m_guns[i].draw(target);
	}

	for (size_t j = 0; j < this->m_bullets.size(); j++)
	{

	}
}

//BossGun Class

BossGun::BossGun(sf::Texture* texture, sf::Vector2f position, float shootTimerMax)
{
	this->m_shootTimerMax = shootTimerMax;
	this->m_shootTimer = this->m_shootTimerMax;

	this->m_gunSprite.setTexture(*texture);
	this->m_gunSprite.setPosition(position);
}

BossGun::~BossGun()
{
}

void BossGun::update(const float & deltaTime)
{
	
}

void BossGun::draw(sf::RenderTarget & target)
{
	target.draw(this->m_gunSprite);
}
