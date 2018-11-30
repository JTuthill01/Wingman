#pragma once

#include "Bullet.h"

class BossGun
{
public:
	BossGun(sf::Texture* texture, sf::Vector2f position, float shootTimerMax);
	~BossGun();

	void update(const float &deltaTime);
	void draw(sf::RenderTarget &target);

private:
	float m_shootTimer;
	float m_shootTimerMax;

	float m_deltaTimeMultiplier;

	sf::Sprite m_gunSprite;
};

class Boss
{
public:
	Boss(dArr<sf::Texture> &bodyTextures, dArr<sf::Texture> &gunTextures, dArr<sf::Texture> &bulletTextures, sf::Vector2f position, int type);
	virtual ~Boss();

	void movement();
	void update(const float &deltaTime);
	void draw(sf::RenderTarget &target);

private:
	int m_nrOfGuns;
	int m_type;

	float m_deltaTimeMultiplier;

	sf::Sprite m_sprite;
	dArr<sf::Texture>* m_bodyTextures;
	dArr<sf::Texture>* m_gunTextures;
	dArr<sf::Texture>* m_bulletTextures;
	dArr<BossGun> m_guns;
	dArr<Bullet> m_bullets; 

	int m_damageMin;
	int m_damageMax;

	int m_hpMax;
	int m_hp;

};

