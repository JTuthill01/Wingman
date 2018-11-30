#include "Player.h"

unsigned Player::players = 0;

dArr<sf::Texture> Player::playerBodyTextures;
dArr<sf::Texture> Player::playerBulletTextures;
dArr<sf::Texture> Player::playerMainGunTextures;
dArr<sf::Texture> Player::playerMainGunIceRayTextures;
dArr<sf::Texture> Player::playerIceBulletTextures;
dArr<sf::Texture> Player::lWingTextures;
dArr<sf::Texture> Player::rWingTextures;
dArr<sf::Texture> Player::cPitTextures;
dArr<sf::Texture> Player::auraTextures;
dArr<sf::Texture> Player::playerShieldTextures;


Player::Player(int UP, int DOWN, int LEFT, int RIGHT, int SHOOT, int SHIELD, int Stats)
	: m_level(1), m_exp(0), m_hp(10),
	m_hpMax(10), m_hpAdded(10), m_statPoints(0), m_plating(0), m_wiring(0), m_cooling(0), m_power(0),
	m_damage(1), m_damageMax(2), m_score(0), m_playerHp(10)
{
	//Guns
	this->m_currentweapon = LASER;

	//Delta Time
	this->m_deltaTimeMultiplier = 62.5f;

	//KeyTime
	this->m_keyTimeMax = 8.f;
	this->m_keyTime = this->m_keyTimeMax;

	//Stats
	this->m_expNext = 20 + static_cast<int>((50 / 3) * ((pow(m_level, 3) -
		6 * pow(m_level, 2)) + 17 * m_level - 12));

	//Update positions
	this->m_playerCenter.x = this->m_sprite.getPosition().x + this->m_sprite.getGlobalBounds().width / 2;
	this->m_playerCenter.y = this->m_sprite.getPosition().y + this->m_sprite.getGlobalBounds().height / 2;

	this->m_sprite.setTexture(Player::playerBodyTextures[0]);
	this->m_sprite.setScale(0.1f, 0.1f);
	this->m_sprite.setColor(sf::Color(10, 10, 10, 255));
	this->m_sprite.setPosition(100.f, 100.f);


	this->m_mainGunSprite.setTexture(Player::playerMainGunTextures[0]);

	this->m_mainGunSprite.setOrigin(this->m_mainGunSprite.getGlobalBounds().width / 2,
		this->m_mainGunSprite.getGlobalBounds().height / 2);

	this->m_mainGunSprite.rotate(90);

	this->m_mainGunSprite.setPosition(this->m_playerCenter.x + 20.f, this->m_playerCenter.y);

	this->m_shieldSprite.setTexture(Player::playerShieldTextures[0]);

	this->m_shieldSprite.setOrigin(this->m_shieldSprite.getGlobalBounds().width / 2,
		this->m_shieldSprite.getGlobalBounds().height / 2);

	this->m_shieldSprite.setPosition(this->m_playerCenter);

	//Accessories

	//Selectors
	this->m_lWingSelect = 9;
	this->m_rWingSelect = 9;
	this->m_cPitSelect = 11;
	this->m_auraSelect = 11;

	//Accessories Textures
	this->m_lWing.setTexture(Player::lWingTextures[this->m_rWingSelect]);
	this->m_rWing.setTexture(Player::rWingTextures[this->m_rWingSelect]);
	this->m_cPit.setTexture(Player::cPitTextures[this->m_cPitSelect]);
	this->m_aura.setTexture(Player::auraTextures[this->m_auraSelect]);

	//Init Accessories

	//Left Wing
	this->m_lWing.setOrigin(this->m_lWing.getGlobalBounds().width / 2,
		this->m_lWing.getGlobalBounds().height / 2);

	this->m_lWing.setPosition(this->m_playerCenter);
	this->m_lWing.setRotation(90.f);
	this->m_lWing.setScale(0.6f, 0.6f);

	//Right Wing
	this->m_rWing.setOrigin(this->m_rWing.getGlobalBounds().width / 2,
		this->m_rWing.getGlobalBounds().height / 2);

	this->m_rWing.setPosition(this->m_playerCenter);
	this->m_rWing.setRotation(90.f);
	this->m_rWing.setScale(0.6f, 0.6f);

	//Cockpit
	this->m_cPit.setOrigin(this->m_cPit.getGlobalBounds().width / 2,
		this->m_cPit.getGlobalBounds().height / 2);

	this->m_cPit.setPosition(this->m_playerCenter);
	this->m_cPit.setRotation(90.f);
	//this->m_cPit.setScale(0.7f, 0.7f);

	//Aura
	this->m_aura.setOrigin(this->m_aura.getGlobalBounds().width / 2,
		this->m_aura.getGlobalBounds().height / 2);

	this->m_aura.setPosition(this->m_playerCenter);
	this->m_aura.setRotation(45.f);
	this->m_aura.setScale(1.7f, 1.7f);
	//this->m_aura.setColor(sf::Color(0, 255, 127, 255));

	//Timers
	this->m_shootTimerMax = 25.f;
	this->m_shootTimer = this->m_shootTimerMax;

	this->m_damageTimerMax = 40.f;
	this->m_damageTimer = this->m_damageTimerMax;

	this->m_shieldTimerMax = 50.f + this->m_cooling + (this->m_wiring / 2);
	this->m_shieldTimer = this->m_shieldTimerMax;

	this->m_shieldRechargeTimerMax = 100.f;
	this->m_shieldRechargeTimer = this->m_shieldRechargeTimerMax;

	//Controls P1
	this->m_controls.add(int(UP));
	this->m_controls.add(int(DOWN));
	this->m_controls.add(int(LEFT));
	this->m_controls.add(int(RIGHT));
	this->m_controls.add(int(SHOOT));
	this->m_controls.add(int(SHIELD));
	this->m_controls.add(int(Stats));

	this->m_maxVelocity = 20.0f;
	this->m_acceleration = 0.8f;
	this->m_stablizierForce = 0.55f;

	this->m_isShielding = false;

	//Upgrades 
	this->m_mainGunLevel = 0;
	this->m_piercingShot = false;
	this->m_iceRay = false;
	this->m_shield = false;
	this->m_dualMissiles01 = false;
	this->m_dualMissiles02 = false;

	this->setGunLevel(0);

	//Add players
	this->m_playerNr = Player::players;
	Player::players++;
}

Player::~Player()
{
}

Bullet & Player::getBullet(unsigned index)
{
	if (index < 0 || index > this->m_bullets.size())
		throw "Out of bounds! Player::GetBullet";

	return this->m_bullets[index];
}

void Player::removeBullet(unsigned index)
{
	if (index < 0 || index > this->m_bullets.size())
		throw "Out of bounds! Player::RemoveBullet";

	this->m_bullets.remove(index);
}

void Player::takeDamage(int damage)
{
	this->m_hp -= damage;

	this->m_damageTimer = 0;

	this->m_currentVelocity.x += -this->normalDirection.x * 10.f;
	this->m_currentVelocity.y += -this->normalDirection.y * 10.f;
}

void Player::gainHp(int hp)
{
	this->m_hp += hp;

	if (this->m_hp > this->m_hpMax)
		this->m_hp = this->m_hpMax;
}

void Player::setGunLevel(int gunLevel)
{
	this->m_mainGunLevel = gunLevel;

	if (this->m_mainGunLevel < Player::playerMainGunTextures.size())
		this->m_mainGunSprite.setTexture(Player::playerMainGunTextures[this->m_mainGunLevel]);
	else
		std::cout << "No fucking texture was found!! Main Gun" << "\n";
}

void Player::addStatPointRandom()
{
}

bool Player::playerShowStatsIsPressed()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->m_controls[controls::STATS])))
		return true;

	return false;
}

std::string Player::getStatsAsString() const
{
	return
		"Level: " + std::to_string(this->m_level) +
		"\n Exp: " + std::to_string(this->m_exp) + " \ " +
		std::to_string(this->m_hpMax) + " ( " + std::to_string(this->m_hpAdded) + " ) " +
		"\n Damage: " + std::to_string(this->m_damage) + " \ " + std::to_string(this->m_damageMax) +
		"\n \n Score: " + std::to_string(this->m_score) +
		"\n \n Power: " + std::to_string(this->m_power) +
		"\n Plating: " + std::to_string(this->m_plating) +
		"\n Wiring: " + std::to_string(this->m_wiring) +
		"\n Cooling: " + std::to_string(this->m_cooling);
}

void Player::reset()
{
	//Reset sprites
	this->m_sprite.setPosition(sf::Vector2f(400.f, 300.f + m_playerNr * 150.f));

	//Reset stats
	this->m_hpMax = 10;
	this->m_hp = this->m_hpMax;
	this->m_wiring = 0;
	this->m_cooling = 0;
	this->m_power = 0;
	this->m_plating = 0;
	this->m_level = 1;
	this->m_exp = 0;
	this->m_expNext = 0;
	this->m_score = 0;

	//Reset physics and movement
	this->m_currentVelocity.x = 0;
	this->m_currentVelocity.y = 0;

	//Reset arrays
	this->m_bullets.clear();

	//Reset upgrades
	this->m_dualMissiles01 = false;
	this->m_dualMissiles02 = false;
	this->m_shield = false;
	this->m_piercingShot = false;
	this->m_iceRay = false;

	this->setGunLevel(0);
	this->m_currentweapon = LASER;

	this->m_shootTimer = this->m_shootTimerMax;
	this->m_damageTimer = this->m_damageTimerMax;
}

bool Player::updateLeveling()
{
	if (this->m_exp >= this->m_expNext)
	{
		this->m_level++;
		this->m_statPoints++;
		this->m_exp -= this->m_expNext;
		this->m_expNext = static_cast<int>((50 / 3) * ((pow(m_level, 3) -
			6 * pow(m_level, 2)) + 17 * m_level - 12));

		this->m_wiring++;
		this->m_cooling++;
		this->m_plating++;
		this->m_power++;

		this->updateStats();

		this->m_hp = m_hpMax;

		return true;
	}

	return false;
}

void Player::updateStats()
{
	this->m_playerHp++;
	this->m_hpMax = this->m_hpAdded + this->m_plating * 5;
	this->m_damageMax = 2 + this->m_power * 2;
	this->m_damage = 1 + this->m_power;
	this->m_shieldTimerMax = 50.f + this->m_cooling + (this->m_wiring / 2);
}

void Player::addStatPointsRandom()
{
	int random = rand() % 4;

	switch (random)
	{
	case 0:
		this->m_power++;
		break;

	case 1:
		this->m_wiring++;
		break;

	case 2:
		this->m_cooling++;
		break;

	case 3:
		this->m_plating++;
		break;

	default:
		break;
	}
	this->updateStats();
}

void Player::draw(sf::RenderTarget &target)
{
	target.draw(this->m_aura);

	for (size_t i = 0; i < this->m_bullets.size(); i++)
	{
		this->m_bullets[i].draw(target);
	}

	for (size_t j = 0; j < this->m_iceBullets.size(); j++)
	{
		this->m_iceBullets[j].draw(target);
	}

	target.draw(this->m_mainGunSprite);

	target.draw(this->m_sprite);

	target.draw(this->m_cPit);

	target.draw(this->m_lWing);

	target.draw(this->m_rWing);

	if (this->m_isShielding)
		target.draw(this->m_shieldSprite);
}

void Player::update(sf::Vector2u windowBounds, const float &deltaTime)
{
	//Update Timers
	if (this->m_shootTimer < this->m_shootTimerMax)
		this->m_shootTimer += 1.f * deltaTime * this->m_deltaTimeMultiplier;

	if (this->m_damageTimer < this->m_damageTimerMax)
		this->m_damageTimer += 1.f * deltaTime * this->m_deltaTimeMultiplier;

	if (this->m_shieldRechargeTimer < this->m_shieldRechargeTimerMax)
		this->m_shieldRechargeTimer += 1.f * deltaTime * this->m_deltaTimeMultiplier;

	if (this->m_shieldTimer < this->m_shieldTimerMax
		&& this->m_shieldRechargeTimer >= this->m_shieldRechargeTimerMax)
		this->m_shieldTimer += 1.f * deltaTime * this->m_deltaTimeMultiplier;

	if (this->m_keyTime < this->m_keyTimeMax)
		this->m_keyTime += 1.f * deltaTime * this->m_deltaTimeMultiplier;

	this->movement(windowBounds, deltaTime);
	this->changeAccessories(deltaTime);
	this->updateAcessories(deltaTime);
	this->combat(deltaTime);
}

void Player::movement(sf::Vector2u windowBounds, const float &deltaTime)
{
	this->normalDirection = normalize(this->m_currentVelocity, vectorLength(this->m_currentVelocity));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->m_controls[controls::UP])))
	{
		this->m_direction.x = 0.0f;
		this->m_direction.y = -1.5f;

		if (this->m_currentVelocity.y > -this->m_maxVelocity && this->m_direction.y < 0)
			this->m_currentVelocity.y += this->m_direction.y * this->m_acceleration
			* deltaTime * this->m_deltaTimeMultiplier;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->m_controls[controls::DOWN])))
	{
		this->m_direction.x = 0.0f;
		this->m_direction.y = 1.5f;

		if (this->m_currentVelocity.y < this->m_maxVelocity && this->m_direction.y > 0)
			this->m_currentVelocity.y += this->m_direction.y * this->m_acceleration
			* deltaTime * this->m_deltaTimeMultiplier;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->m_controls[controls::LEFT])))
	{
		this->m_direction.x = -1.5f;
		this->m_direction.y = 0.0f;

		if (this->m_currentVelocity.x > -this->m_maxVelocity && this->m_direction.x < 0)
			this->m_currentVelocity.x += this->m_direction.x * this->m_acceleration
			* deltaTime * this->m_deltaTimeMultiplier;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->m_controls[controls::RIGHT])))
	{
		this->m_direction.x = 1.5f;
		this->m_direction.y = 0.0f;

		if (this->m_currentVelocity.x < this->m_maxVelocity && this->m_direction.x > 0)
			this->m_currentVelocity.x += this->m_direction.x * this->m_acceleration
			* deltaTime * this->m_deltaTimeMultiplier;
	}

	//Drag Force
	if (this->m_currentVelocity.x > 0)
	{
		this->m_currentVelocity.x -= this->m_stablizierForce
			* deltaTime * this->m_deltaTimeMultiplier;

		if (this->m_currentVelocity.x < 0)
			this->m_currentVelocity.x = 0;
	}
	else if (this->m_currentVelocity.x < 0)
	{
		this->m_currentVelocity.x += this->m_stablizierForce
			* deltaTime * this->m_deltaTimeMultiplier;


		if (this->m_currentVelocity.x > 0)
			this->m_currentVelocity.x = 0;
	}

	if (this->m_currentVelocity.y > 0)
	{
		this->m_currentVelocity.y -= this->m_stablizierForce
			* deltaTime * this->m_deltaTimeMultiplier;

		if (this->m_currentVelocity.y < 0)
			this->m_currentVelocity.y = 0;
	}
	else if (this->m_currentVelocity.y < 0)
	{
		this->m_currentVelocity.y += this->m_stablizierForce
			* deltaTime * this->m_deltaTimeMultiplier;

		if (this->m_currentVelocity.y > 0)
			this->m_currentVelocity.y = 0;
	}

	//Final Movement
	this->m_sprite.move(this->m_currentVelocity.x * deltaTime * this->m_deltaTimeMultiplier,
		this->m_currentVelocity.y * deltaTime * this->m_deltaTimeMultiplier);

	//Update positions
	this->m_playerCenter.x = this->m_sprite.getPosition().x +
		this->m_sprite.getGlobalBounds().width / 2;
	this->m_playerCenter.y = this->m_sprite.getPosition().y +
		this->m_sprite.getGlobalBounds().height / 2;

	//Window Bounds

	//Left Side
	if (this->getPosition().x <= 0)
	{
		this->m_sprite.setPosition(0.f, this->m_sprite.getPosition().y);
		this->m_currentVelocity.x = 0.f;
	}

	//Top
	if (this->getPosition().y <= 0)
	{
		this->m_sprite.setPosition(this->m_sprite.getPosition().x, 0.f);
		this->m_currentVelocity.y = 0.f;
	}

	//Right Side
	else if (this->getPosition().x + this->getGobalBounds().width >= windowBounds.x)
	{
		this->m_sprite.setPosition(windowBounds.x -
			this->getGobalBounds().width, this->m_sprite.getPosition().y);

		this->m_currentVelocity.x = 0.f;
	}

	//Bottom
	else if (this->getPosition().y + this->getGobalBounds().height >= windowBounds.y)
	{
		this->m_sprite.setPosition(this->m_sprite.getPosition().x,
			windowBounds.y - this->getGobalBounds().height);

		this->m_currentVelocity.y = 0.f;
	}
}

void Player::combat(const float &deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->m_controls[controls::SHOOT]))
		&& this->m_shootTimer >= this->m_shootTimerMax)
	{
		if (this->m_currentweapon == LASER)
		{
			if (this->m_mainGunLevel == 0)
			{
				//Create Bullet
				this->m_bullets.add(Bullet(&Player::playerBulletTextures[LASER], sf::Vector2f(this->m_playerCenter.x + 55.f, this->m_playerCenter.y), sf::Vector2f(.21f, .21f), sf::Vector2f(1.0f, 0.0f), 20.5f,
					60.23f, 5.63f));
			}

			else if (this->m_mainGunLevel == 1)
			{
				this->m_bullets.add(Bullet(&Player::playerBulletTextures[LASER], sf::Vector2f(this->m_playerCenter.x + 100.f,
					this->m_playerCenter.y - 15.f), sf::Vector2f(0.2f, 0.2f), sf::Vector2f(1.f, 0.f),
					20.f, 60.f, 5.f));

				this->m_bullets.add(Bullet(&Player::playerBulletTextures[LASER], sf::Vector2f(this->m_playerCenter.x + 100.f,
					this->m_playerCenter.y + 15.f), sf::Vector2f(0.2f, 0.2f), sf::Vector2f(1.f, 0.f),
					20.f, 60.f, 5.f));
			}

			else if (this->m_mainGunLevel == 2)
			{
				this->m_bullets.add(Bullet(&Player::playerBulletTextures[LASER], sf::Vector2f(this->m_playerCenter.x + 100.f,
					this->m_playerCenter.y - 40.f), sf::Vector2f(0.2f, 0.2f), sf::Vector2f(1.f, 0.f),
					20.f, 60.f, 5.f));

				this->m_bullets.add(Bullet(&Player::playerBulletTextures[LASER], sf::Vector2f(this->m_playerCenter.x + 100.f,
					this->m_playerCenter.y), sf::Vector2f(0.2f, 0.2f), sf::Vector2f(1.f, 0.f),
					20.f, 60.f, 5.f));

				this->m_bullets.add(Bullet(&Player::playerBulletTextures[LASER], sf::Vector2f(this->m_playerCenter.x + 100.f,
					this->m_playerCenter.y + 40.f), sf::Vector2f(0.2f, 0.2f), sf::Vector2f(1.f, 0.f),
					20.f, 60.f, 5.f));
			}

			//Animate Gun
			this->m_mainGunSprite.move(-40.f, 0.f);

		}

		else if (this->m_currentweapon == MISSILE01)
		{
			//Create Bullet
			this->m_bullets.add(Bullet(&Player::playerBulletTextures[MISSILE01], sf::Vector2f(this->m_playerCenter.x, this->m_playerCenter.y - 25.f), sf::Vector2f(0.08f, 0.08f), sf::Vector2f(1.0f, 0.0f), 2.5f,
				50.23f, 1.65f));

			if (this->m_dualMissiles01)
			{
				//Create Bullet
				this->m_bullets.add(Bullet(&Player::playerBulletTextures[MISSILE01], sf::Vector2f(this->m_playerCenter.x, this->m_playerCenter.y + 25.f), sf::Vector2f(0.08f, 0.08f), sf::Vector2f(1.0f, 0.0f), 2.5f,
					50.23f, 1.65f));
			}
		}

		else if (this->m_currentweapon == MISSILE02)
		{
			if (this->m_dualMissiles02)
			{

			}
		}

		else if (this->m_currentweapon == ICERAY01)
		{
			/*this->m_iceBullets.add(Bullet(&Player::playerIceBulletTextures[ICERAY01], sf::Vector2f(this->m_playerCenter.x + 55.f, this->m_playerCenter.y), sf::Vector2f(1.21f, 1.21f), sf::Vector2f(1.0f, 0.0f), 20.5f,
				60.23f, 5.63f));*/
		}

		this->m_mainGunSprite.move(-40.f, 0.f);

		//Reset Timer
		this->m_shootTimer = 0;
	}

	//Shielding
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->m_controls[controls::SHIELD])))
	{
		if (this->m_shieldTimer > 0 && this->m_shieldRechargeTimer >= this->m_shieldRechargeTimerMax)
		{
			this->m_isShielding = true;
			this->m_shieldTimer -= 2.f * deltaTime * this->m_deltaTimeMultiplier;

			if (this->m_shieldTimer <= 0.f)
				this->m_shieldRechargeTimer = 0.f;
		}
		else
		{
			this->m_isShielding = false;
		}
	}
	else
	{
		this->m_isShielding = false;
	}

	if (this->isDamageCoolDown())
	{
		if ((int)this->m_damageTimer % 2 == 0)
		{
			this->m_lWing.setColor(sf::Color::Red);
			this->m_rWing.setColor(sf::Color::Red);
			this->m_cPit.setColor(sf::Color::Red);
		}

		else
		{
			this->m_lWing.setColor(sf::Color::White);
			this->m_rWing.setColor(sf::Color::White);
			this->m_cPit.setColor(sf::Color::White);
		}
	}

	else
	{
		this->m_lWing.setColor(sf::Color::White);
		this->m_rWing.setColor(sf::Color::White);
		this->m_cPit.setColor(sf::Color::White);
	}

}

void Player::updateAcessories(const float &deltaTime)
{
	this->m_mainGunSprite.setPosition(this->m_mainGunSprite.getPosition().x, this->m_playerCenter.y);

	//Set Shield Position
	this->m_shieldSprite.setPosition(this->m_playerCenter.x,
		this->m_playerCenter.y + 20.f);

	if (this->m_mainGunSprite.getPosition().x < this->m_playerCenter.x + 20.f)
	{
		this->m_mainGunSprite.move(5.f * deltaTime * this->m_deltaTimeMultiplier
			+ this->m_currentVelocity.x * deltaTime * this->m_deltaTimeMultiplier, 0.f);
	}

	if (this->m_mainGunSprite.getPosition().x > this->m_playerCenter.x + 20.f)
	{
		this->m_mainGunSprite.setPosition(this->m_playerCenter.x + 20.f, this->m_playerCenter.y);
	}

	//Left Wing
	this->m_lWing.setPosition(m_playerCenter.x + -abs(this->m_currentVelocity.x),
		m_playerCenter.y + -abs(this->m_currentVelocity.x / 2));

	//Right Wing
	this->m_rWing.setPosition(m_playerCenter.x + -abs(this->m_currentVelocity.x),
		m_playerCenter.y + abs(this->m_currentVelocity.x / 2));

	//Cockpit
	this->m_cPit.setPosition(m_playerCenter.x + this->m_currentVelocity.x,
		m_playerCenter.y);

	//Aura
	this->m_aura.setPosition(m_playerCenter);
	this->m_aura.rotate(7.f * deltaTime * this->m_deltaTimeMultiplier);


}

void Player::changeAccessories(const float &deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1) && this->m_keyTime >= this->m_keyTimeMax)
	{
		if (m_lWingSelect < Player::lWingTextures.size() - 1)
		{
			m_lWingSelect++;
		}
		else
			this->m_lWingSelect = 0;

		this->m_lWing.setTexture(Player::lWingTextures[m_lWingSelect]);

		this->m_keyTime = 0.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2) && this->m_keyTime >= this->m_keyTimeMax)
	{
		if (m_rWingSelect < Player::rWingTextures.size() - 1)
		{
			m_rWingSelect++;
		}
		else
			this->m_rWingSelect = 0;

		this->m_rWing.setTexture(Player::rWingTextures[m_rWingSelect]);

		this->m_keyTime = 0.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3) && this->m_keyTime >= this->m_keyTimeMax)
	{
		if (m_cPitSelect < Player::cPitTextures.size() - 1)
		{
			m_cPitSelect++;
		}
		else
			this->m_cPitSelect = 0;

		this->m_cPit.setTexture(Player::cPitTextures[m_cPitSelect]);

		this->m_keyTime = 0.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4) && this->m_keyTime >= this->m_keyTimeMax)
	{
		if (m_auraSelect < Player::auraTextures.size() - 1)
		{
			m_auraSelect++;
		}
		else
			this->m_auraSelect = 0;

		this->m_aura.setTexture(Player::auraTextures[m_auraSelect]);

		this->m_keyTime = 0.f;
	}
}

const int Player::getDamage() const
{
	int damage = 0;
	switch (this->m_currentweapon)
	{
	case LASER:
		damage = rand() % this->m_damageMax + this->m_damage;
		break;

	case MISSILE01:
		damage = rand() % this->m_damageMax + this->m_damage;
		damage *= 2;

		break;

	case MISSILE02:
		damage = rand() % this->m_damageMax + this->m_damage;
		damage *= 4;

		break;

	case ICERAY01:
		damage = rand() % this->m_damageMax + this->m_damage;
		damage *= 2;

		break;

	default:
		damage = rand() % this->m_damageMax + this->m_damage;

		break;
	}

	return damage;
}

void Player::auraChange()
{
	if (this->m_hp > 6 && this->m_hp <= 10)
	{
		this->m_aura.setColor(sf::Color(248, 248, 255, 255));
	}

	else if (this->m_hp >= 30)
	{
		this->m_aura.setColor(sf::Color(128, 0, 128, 255));
	}

	else if (this->m_hp >= 20 && this->m_hp < 25)
	{
		this->m_aura.setColor(sf::Color(135, 206, 250, 255));
	}

	else if (this->m_hp > 10 && this->m_hp < 20)
	{
		this->m_aura.setColor(sf::Color(127, 255, 0, 255));
	}

	else if (this->m_hp <= 6 && this->m_hp > 3)
	{
		this->m_aura.setColor(sf::Color(255, 69, 100, 255));
	}

	else if (this->m_hp <= 3)
	{
		this->m_aura.setColor(sf::Color::Black);
	}
}

void Player::setCurrentWeapon()
{
	if (this->m_iceRay)
	{
		this->m_mainGunSprite.setTexture(Player::playerMainGunIceRayTextures[0]);
		this->m_currentweapon = ICERAY01; 
	}

	else if (!this->m_iceRay)
	{
		this->m_mainGunSprite.setTexture(Player::playerMainGunTextures[getCurrentWeapon()]);
	}
}


