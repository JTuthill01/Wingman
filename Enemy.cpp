#include "Enemy.h"

enum eTypes { MOVELEFT = 0, FOLLOW, MOVELEFTSHOOT, FOLLOWFAST, FOLLOWSHOOT, FOLLOWFASTSHOOT };
enum bulletTypes { REGULAR = 0, MISSILES };

dArr<Bullet> Enemy::enemyBullets;

Enemy::Enemy(dArr<sf::Texture> &textures, dArr<sf::Texture> &bulletTextures, sf::Vector2u windowBounds,
	sf::Vector2f position, sf::Vector2f moveDirection, int type,
	int scalar, int playerFollowNr)
{
	this->m_deltaTimeMultiplier = 62.5f;

	this->m_textures = &textures;
	this->m_bulletTextures = &bulletTextures;
	this->m_type = type;

	if (this->m_type >= (this->m_textures)->size())
		std::cout << "No Textures found" << "\n";

	else
		this->m_sprite.setTexture((*this->m_textures)[this->m_type]);

	this->m_sprite.setOrigin(this->m_sprite.getGlobalBounds().width / 2,
		this->m_sprite.getGlobalBounds().height / 2);

	this->m_windowBounds = windowBounds;
	
	this->m_moveDirection = moveDirection;

	//Timers
	this->m_damageTimerMax = 4.f;
	this->m_damageTimer = 0.f;
	this->m_shootTimerMax = 50.f;
	this->m_shootTimer = this->m_shootTimerMax;

	this->m_hpMax = m_hpMax;
	this->m_hp = this->m_hp;

	this->m_damageMax = m_damageMax;
	this->m_damageMin = m_damageMin;

	this->m_stopMovement = true;

	switch (this->m_type)
	{
	case MOVELEFT:
		this->m_sprite.setScale(sf::Vector2f(0.1f, 0.1f));
		this->m_hpMax = (rand() % 5 + 1) * scalar;
		this->m_hp = this->m_hpMax;
		this->m_damageMax = (rand() % 3 + 1) * scalar;
		this->m_damageMin = (rand() % 1 + 1) * scalar;
		this->m_maxVelocity = rand() % 20 + 5;
		break;

	case FOLLOW:
		this->m_sprite.setScale(sf::Vector2f(0.15f, 0.15f));
		this->m_hpMax = (rand() % 3 + 1) * scalar;
		this->m_hp = this->m_hpMax;
		this->m_damageMax = (rand() % 2 + 1) * scalar;
		this->m_damageMin = (rand() % 1 + 1) * scalar;
		this->m_maxVelocity = 6 + 3;
		break;

	case MOVELEFTSHOOT:
		this->m_sprite.setScale(sf::Vector2f(0.17f, 0.17f));
		this->m_hpMax = (rand() % 3 + 1) * scalar;
		this->m_hp = this->m_hpMax;
		this->m_damageMax = (rand() % 2 + 1) * scalar;
		this->m_damageMin = (rand() % 1 + 1) * scalar;
		this->m_maxVelocity = 10 + 3;
		this->m_shootTimerMax = 15.f;
		this->m_shootTimer = 0.f; 
		break;

	default:
		this->m_hpMax = (rand() % 3 + 1) * scalar;
		this->m_hp = this->m_hpMax;
		this->m_damageMax = (rand() % 2 + 1) * scalar;
		this->m_damageMin = (rand() % 1 + 1) * scalar;
		this->m_maxVelocity = 15 + 5;
		break;
	}

	this->m_playerFollowNr = playerFollowNr; 

	this->m_sprite.setPosition(windowBounds.x, (rand() % windowBounds.y) - 
		this->m_sprite.getGlobalBounds().height);
}

Enemy::~Enemy()
{
}

void Enemy::takeDamage(int damage)
{
	this->m_hp -= damage;

	this->m_damageTimer = this->m_damageTimerMax; 

	if (this->m_hp <= 0)
		this->m_hp = 0;
}

void Enemy::update(const float &deltaTime, sf::Vector2f playerPosition)
{
	switch (this->m_type)
	{
	case MOVELEFT:	
		this->m_sprite.move(this->m_moveDirection.x * this->m_maxVelocity * deltaTime * this->m_deltaTimeMultiplier,
			this->m_moveDirection.y * this->m_maxVelocity * deltaTime * this->m_deltaTimeMultiplier);

		this->m_normalizedMoveDirection = normalize(this->m_moveDirection, vectorLength(this->m_moveDirection));
		break;

	case FOLLOW:
		if (this->m_sprite.getPosition().x > playerPosition.x)
		{
			this->m_moveDirection.x = playerPosition.x - this->m_sprite.getPosition().x;
			this->m_moveDirection.y = playerPosition.y - this->m_sprite.getPosition().y;
		}

		this->m_normalizedMoveDirection = normalize(this->m_moveDirection, vectorLength(this->m_moveDirection));
		
		if (this->m_normalizedMoveDirection.y > 0.3f)
			this->m_normalizedMoveDirection.y = 0.3f;

		else if (this->m_normalizedMoveDirection.y < -0.3f)
			this->m_normalizedMoveDirection.y = -0.3f;

		if (this->m_normalizedMoveDirection.x > -0.7f)
			this->m_normalizedMoveDirection.x = -0.7f;

		this->m_sprite.setRotation(atan2(this->m_normalizedMoveDirection.y, this->m_normalizedMoveDirection.x) * 180 / thor::Pi + 180);

		this->m_sprite.move(this->m_normalizedMoveDirection.x * this->m_maxVelocity * deltaTime * this->m_deltaTimeMultiplier,
			this->m_normalizedMoveDirection.y * this->m_maxVelocity * deltaTime * this->m_deltaTimeMultiplier);

		break;

	case MOVELEFTSHOOT:
		this->m_shootTimerMax = 50.f;

		if (this->m_shootTimer < this->m_shootTimerMax)
			this->m_shootTimer += 1.f * deltaTime * this->m_deltaTimeMultiplier;

			this->m_lookDirection.x = playerPosition.x - this->m_sprite.getPosition().x;
			this->m_lookDirection.y = playerPosition.y - this->m_sprite.getPosition().y;
		
		this->m_normalizedLookDirection = normalize(this->m_lookDirection,
			vectorLength(this->m_lookDirection));

		this->m_sprite.setRotation(atan2(this->m_normalizedLookDirection.y,
			this->m_normalizedLookDirection.x) * 180 / thor::Pi + 180);

		this->m_sprite.move(this->m_moveDirection.x * this->m_maxVelocity * deltaTime * this->m_deltaTimeMultiplier,
			this->m_moveDirection.y * this->m_maxVelocity * deltaTime * 
			this->m_deltaTimeMultiplier);

		this->m_normalizedMoveDirection = normalize(this->m_moveDirection, 
			vectorLength(this->m_moveDirection));

		//Shoot
		if (this->m_shootTimer >= this->m_shootTimerMax)
		{
			//Create Bullet
			Enemy::enemyBullets.add(Bullet(&(*this->m_bulletTextures)[REGULAR], this->m_sprite.getPosition(), sf::Vector2f(0.2f, 0.2f), this->m_normalizedLookDirection, 2.f,
				5, 0.5f));
			this->m_shootTimer = 0.f;

			this->m_shootTimer = 0.f;
		}

		break;

	default:
		break;
	}

	//Damaged
	if (this->m_damageTimer > 0.f)
	{
		this->m_damageTimer -= 1.f * deltaTime * this->m_deltaTimeMultiplier;

		this->m_sprite.setColor(sf::Color(178, 34, 34, 255));

		this->m_sprite.move(10.f * -this->m_normalizedMoveDirection.x * this->m_damageTimer
			* deltaTime * this->m_deltaTimeMultiplier,
			10.f * -this->m_normalizedMoveDirection.y * this->m_damageTimer
			* deltaTime * this->m_deltaTimeMultiplier);
	}
	else
		this->m_sprite.setColor(sf::Color::White);
}

void Enemy::draw(sf::RenderTarget & target)
{
	target.draw(m_sprite);
}

void Enemy::collision()
{
	this->m_damageTimer = this->m_damageTimerMax;
}

void Enemy::stopMovement()
{
	this->m_moveDirection = sf::Vector2f(0.f, 6.f);
}

void Enemy::resumeMovement()
{
	this->m_moveDirection = sf::Vector2f(-1.f, 0.f);
}



