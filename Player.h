#pragma once
#include "Bullet.h"

enum controls { UP = 0, DOWN, LEFT, RIGHT, SHOOT, SHIELD, STATS };

enum weapons { LASER = 0, MISSILE01, MISSILE02, ICERAY01 };

class Player
{
public:
	Player(int UP = sf::Keyboard::W, int DOWN = sf::Keyboard::S, 
		int LEFT = sf::Keyboard::A, int RIGHT = sf::Keyboard::D, 
		int SHOOT = sf::Keyboard::Space, int SHIELD = sf::Keyboard::RAlt, int Stats = sf::Keyboard::Tab);

	virtual ~Player();

	//Accessors
	Bullet& getBullet(unsigned index);
	void removeBullet(unsigned index);
	inline const int getBulletSize()const { return this->m_bullets.size(); }
	inline const sf::Vector2f& getPosition()const { return this->m_sprite.getPosition(); }
	inline const sf::String getHpAsString()const { return std::to_string(this->m_hp) + "/" + std::to_string(this->m_hpMax);  }
	inline sf::FloatRect getGobalBounds()const { return this->m_sprite.getGlobalBounds(); }
	inline const bool intersects(sf::FloatRect rect)const { return this->m_sprite.getGlobalBounds().intersects(rect); }
	inline const int& getHp()const { return this->m_hp; }
	inline const int& getHpMax()const { return this->m_hpMax; }
	void takeDamage(int damage);
	inline bool isAlive()const { return this->m_hp > 0; }
	inline const int& getPlayerNr() { return this->m_playerNr; }
	inline const int& getLevel()const { return this->m_level; }
	inline const int& getCurrentWeapon()const { return this->m_currentweapon; }
	inline const int& getExp()const { return this->m_exp; }
	inline const int& getExpNext()const { return this->m_expNext; }
	inline bool gainExp(int exp) { this->m_exp += exp; return this->updateLeveling(); }
	void gainHp(int hp);
	void setGunLevel(int gunLevel);
	inline void enablePierchingShot() { this->m_piercingShot = true; }
	inline void disablePierchingShot() { this->m_piercingShot = false; }
	inline void enableShield() { this->m_shield = true; }
	inline void enableDualMissiles01() { this->m_dualMissiles01 = true; }
	inline void enableDualMissiles02() { this->m_dualMissiles02 = true; }
	inline void enableIceRay() { this->m_iceRay = true; }
	inline void disableIceRay() { this->m_iceRay = false; }
	inline void upgradeHp() { this->m_hpAdded += 10; this->updateStats(); this->m_hp = this->m_hpMax; }
	inline bool getPiercingShot()const { return this->m_piercingShot; }
	inline bool getIceShot()const { return this->m_iceRay; }
	inline void gainScore(int score) { this->m_score += score; }
	inline const int getScore()const { return this->m_score; }
	inline bool isDamageCoolDown() { return this->m_damageTimer < this->m_damageTimerMax; }
	inline const int& getGunLevel()const { return this->m_mainGunLevel; }
	inline void move(float x, float y) { this->m_sprite.move(x, y); this->m_mainGunSprite.move(x, y); }
	inline const sf::Vector2f& getNormalDirection()const { return this->normalDirection; }
	inline void resetVelocity() { this->m_currentVelocity = sf::Vector2f(0.f, 0.f); }
	inline void addStatPoints() { this->m_statPoints++; }
	void addStatPointRandom();
	inline dArr<int>& getAquiredUpgrades() { return this->m_upgradesAquired; }
	inline void setPosition(float x, float y) {
		this->m_sprite.setPosition(sf::Vector2f(x, y)); this->m_mainGunSprite.setPosition(sf::Vector2f(x, y));
	}
	inline float getShieldTimer()const { return this->m_shieldTimer; }
	inline float getShieldTimerMax()const { return this->m_shieldTimerMax; }
	inline float getShieldRechargeTimer()const { return this->m_shieldRechargeTimer; }
	inline float getShieldRechargeTimerMax()const { return this->m_shieldRechargeTimerMax; }
	bool playerShowStatsIsPressed();
	std::string getStatsAsString()const;
	inline bool isShielding()const { return this->m_shield; }

	//Functions
	void reset();
	bool updateLeveling();
	void updateStats();
	void addStatPointsRandom();
	void draw(sf::RenderTarget &target);
	void update(sf::Vector2u windowBounds, const float &deltaTime);
	void movement(sf::Vector2u windowBounds, const float &deltaTime);
	void combat(const float &deltaTime);
	void updateAcessories(const float &deltaTime);
	void changeAccessories(const float &deltaTime);
	const int getDamage()const; 
	void auraChange();
	void setCurrentWeapon();

	//Statics
	static unsigned players;

	//Static Textures
	static dArr<sf::Texture> playerBodyTextures;
	static dArr<sf::Texture> playerBulletTextures;
	static dArr<sf::Texture> playerMainGunTextures;
	static dArr<sf::Texture> playerMainGunIceRayTextures;
	static dArr<sf::Texture> playerIceBulletTextures;
	static dArr<sf::Texture> lWingTextures;
	static dArr<sf::Texture> rWingTextures;
	static dArr<sf::Texture> cPitTextures;
	static dArr<sf::Texture> auraTextures;
	static dArr<sf::Texture> playerShieldTextures;


	sf::Vector2f normalDirection;

private:
	float m_deltaTimeMultiplier;
	float m_keyTime;
	float m_keyTimeMax;

	sf::Sprite m_sprite;
	sf::RectangleShape m_hitBox;
	sf::Texture* m_texture;

	int m_lWingSelect;
	int m_rWingSelect;
	int m_cPitSelect;
	int m_auraSelect;

	sf::Sprite m_mainGunSprite;
	sf::Sprite m_mainIceGunSprite;
	sf::Sprite m_shieldSprite;
	sf::Sprite m_lWing;
	sf::Sprite m_rWing;
	sf::Sprite m_cPit;
	sf::Sprite m_aura;

	dArr<Bullet> m_bullets;
	dArr<Bullet> m_iceBullets;

	sf::Vector2f m_playerCenter;

	dArr<int> m_controls;

	//Direction
	sf::Vector2f m_currentVelocity;
	sf::Vector2f m_direction;
	sf::Vector2f m_movementDirection;
	
	float m_maxVelocity;
	float m_acceleration;
	float m_stablizierForce;
	
	int m_level;
	int m_exp;
	int m_expNext;
	int m_statPoints; 

	//Player Upgrades
	int m_cooling;
	int m_plating;
	int m_power;
	int m_wiring;

	//Shield
	bool m_isShielding;

	//Hp
	int m_hp;
	int m_hpMax;
	int m_playerHp;
	int m_hpAdded;

	//Damage
	int m_damage;
	int m_damageMax;

	//Score
	int m_score;

	int m_currentweapon; 

	//Weapon Upgrades
	dArr<int> m_upgradesAquired;
	int m_mainGunLevel;
	bool m_piercingShot;
	bool m_iceRay;
	bool m_shield;
	bool m_dualMissiles01;
	bool m_dualMissiles02; 

	unsigned m_playerNr;

	//Timers
	float m_shootTimer; 
	float m_shootTimerMax;

	float m_damageTimer; 
	float m_damageTimerMax;

	float m_shieldTimer;
	float m_shieldTimerMax;

	float m_shieldRechargeTimer;
	float m_shieldRechargeTimerMax;


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
};

