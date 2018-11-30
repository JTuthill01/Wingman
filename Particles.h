#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <Thor/Math.hpp>
#include <sstream>
#include "dArr.h"

class Particles
{
public:
	Particles(sf::Vector2f position, int textureIndex, sf::Vector2f direction,
		float maxVelocity, float maxRotation, float lifeTime);
	virtual ~Particles();

	//Inline Functions
	inline bool readyToDelete()const { this->m_lifeTime <= 0.f; }

	void update(const float &deltaTime);
	void draw(sf::RenderTarget &target);

	static dArr<sf::Texture> particleTextures;

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

	sf::Vector2f currentVelocity;
	float deceleration;

private:
	float m_deltaTimeMultiplier;

	sf::Sprite m_sprite;
	sf::Vector2f m_direction;
	float m_maxVelocity;
	float m_lifeTime;
	float m_maxRotation;

	dArr<Particles> m_particles;
};

