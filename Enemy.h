#pragma once
#include<SFML/System/Time.hpp>
#include "Bullet.h"

class Enemy
{
public:
	Enemy(dArr<sf::Texture> &textures, dArr<sf::Texture> &bulletTextures, sf::Vector2u windowBounds,
		sf::Vector2f position, sf::Vector2f moveDirection, int type ,int scalar,
		int playerFollowNr);
	~Enemy();

	//Functions
	void takeDamage(int damage);
	void update(const float &deltaTime, sf::Vector2f playerPosition);
	void draw(sf::RenderTarget &target);
	void collision();
	void stopMovement();
	void resumeMovement();

	//Static Shit
	static dArr<Bullet> enemyBullets; 

	//Accessors
	inline const int getDamage()const { return rand() % this->m_damageMax + this->m_damageMin; }
	inline const int getHp()const { return this->m_hp; }
	inline const int getHpMax()const { return this->m_hpMax; }
	inline const bool isDead()const { return this->m_hp <= 0; }
	inline sf::FloatRect getGobalBounds()const { return this->m_sprite.getGlobalBounds(); }
	inline sf::Vector2f getPosition()const { return this->m_sprite.getPosition(); }
	inline const int& getPlayerFollowNr() const { return this->m_playerFollowNr; }
	inline const int& getType()const { return this->m_type; }

private:
	float m_deltaTimeMultiplier;

	dArr<sf::Texture>* m_bulletTextures;

	dArr<sf::Texture>* m_textures;
	sf::Sprite m_sprite;

	sf::Vector2u m_windowBounds;
	sf::Vector2f m_moveDirection;
	sf::Vector2f m_normalizedMoveDirection;
	sf::Vector2f m_lookDirection;
	sf::Vector2f m_normalizedLookDirection;
	bool m_stopMovement;

	float m_maxVelocity; 

	float m_damageTimerMax;
	float m_damageTimer;
	float m_shootTimerMax;
	float m_shootTimer;

	int m_type;
	int m_hp;
	int m_hpMax;
	int m_damageMin;
	int m_damageMax;
	int m_playerFollowNr;

	sf::Clock m_clock;
	sf::Time m_time;
	float m_resumeMovement;

	//Direction Functions

	float vectorLength(sf::Vector2f v)
	{
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}

	sf::Vector2f normalize(sf::Vector2f v, float length)
	{
		if (length == 0)
			return sf::Vector2f(0.f, 0.f);

		else
			return v / length;
	}

	sf::Vector2f move();
};

