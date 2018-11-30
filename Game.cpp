#include "Game.h"

Game::Game(sf::RenderWindow* window)
{
	this->m_window = window;
	this->m_window->setFramerateLimit(400);
	this->m_deltaTimeMultiplier = 62.5f;
	this->m_scoreMultiplier = 1;
	this->m_multiplierAdderMax = 10; 
	this->m_multiplierAdder = 0;
	this->m_multiplierTimerMax = 200.f;
	this->m_multiplierTimer = this->m_multiplierTimerMax;
	this->m_score = 0;
	this->m_scoreTimer.restart();
	this->m_bestScoreSecond = 0.0;
	this->m_difficulty = 0;
	this->m_difficultyTimer = 0.f;

	this->m_paused = false;

	//KeyTime
	this->m_keyTimeMax = 10.f;
	this->m_keyTime = this->m_keyTimeMax;

	this->initialize();
}

Game::~Game()
{
	
}

void Game::updateUIPlayer(int index)
{
	if (index >= this->players.size())
		std::cout << "Out of fucking bounds asshole!!!!" << std::endl; 

	else //Follow Text
	{
		{
			this->m_followPlayerTexts.setPosition(this->players[index].getPosition().x - 20.f,
				this->players[index].getPosition().y - 59.f);
		};

		{
			this->m_followPlayerTexts.setString(std::to_string(this->players[index].getPlayerNr())
				+ "				" + this->players[index].getHpAsString()
				+ "\n\n\n\n\n\n\n" + std::to_string(this->players[index].getLevel()));
		};

		{
			//Exp Bar
			this->m_playerExpBar.setPosition(this->players[index].getPosition().x + 10.f,
				this->players[index].getPosition().y + 90.f);

			//Shield Bar
			this->m_playerShieldBar.setPosition(this->players[index].getPosition().x + 10.f,
				this->players[index].getPosition().y + 115.f);
		};

		{
			this->m_playerExpBar.setScale(static_cast<float>(this->players[index].getExp()) /
				this->players[index].getExpNext(), 1.f);

			if (this->players[index].getShieldRechargeTimer() < this->players[index].getShieldRechargeTimerMax())
			{
				this->m_playerShieldBar.setScale(static_cast<float>(this->players[index].getShieldRechargeTimer()) /
					this->players[index].getShieldRechargeTimerMax(), 1.f);

				this->m_playerShieldBar.setFillColor(sf::Color(255, 140, 0, 255));
			}

			else 
			{
				this->m_playerShieldBar.setScale(static_cast<float>(this->players[index].getShieldTimer()) /
					this->players[index].getShieldTimerMax(), 1.f);

				this->m_playerShieldBar.setFillColor(sf::Color(128, 0, 0, 255));
			}
		};

		//Stats Box
		if (this->players[index].playerShowStatsIsPressed())
		{
			this->m_playerStatsTexts.setString(this->players[index].getStatsAsString());

			this->m_playerStatsTextsBackground.setPosition(this->players[index].getPosition().x,
				this->players[index].getPosition().y + 150.f);

			this->m_playerStatsTextsBackground.setSize(sf::Vector2f(this->m_playerStatsTexts.getGlobalBounds().width,
				this->m_playerStatsTexts.getGlobalBounds().height));

			this->m_playerStatsTexts.setPosition(this->m_playerStatsTextsBackground.getPosition());
		}
	}
}

void Game::updateTimers(const float & deltaTime)
{
	//Key Time Update
	if (this->m_keyTime < this->m_keyTimeMax)
		this->m_keyTime += 1.f * deltaTime * this->m_deltaTimeMultiplier;
}

void Game::updateTimersUnpaused(const float & deltaTime)
{
	//Update Timers
	if (this->m_enemySpawnTimer < this->m_enemySpawnTimerMax)
		this->m_enemySpawnTimer += 1.f * deltaTime * this->m_deltaTimeMultiplier;

	if (this->m_enemySpawnTimerMax > 10)
		this->m_difficultyTimer += 1.f * deltaTime * this->m_deltaTimeMultiplier;

	//Made the game harder with time
	this->m_difficultyTimer += 1.f * deltaTime * this->m_deltaTimeMultiplier;

	//Score Timers + Multipliers
	if (this->m_multiplierTimer > 0.f)
	{
		this->m_multiplierTimer -= 1.f * deltaTime * this->m_deltaTimeMultiplier;

		if (this->m_multiplierTimer <= 0.f)
		{
			this->m_multiplierTimer = 0.f;
			this->m_multiplierAdder = 0;
			this->m_scoreMultiplier = 1;
		}
	}
}

void Game::updateWhilePaused(const float &deltaTime)
{
	if (this->m_paused)
	{
		for (size_t i = 0; i < this->players.size(); i++)
		{
			if (this->players[i].isAlive())
				this->players[i].changeAccessories(deltaTime);
		}
	}

}

void Game::updateDifficulty()
{
	if ((int)this->m_difficultyTimer % 1000 == 0 && this->m_enemySpawnTimerMax > 10)
	{
		if(this->m_enemySpawnTimerMax > 10)
			this->m_enemySpawnTimerMax--;
		
		this->m_difficulty++;
		this->m_difficultyTimer = 1.f;
	}
}

void Game::updateUIEnemy(int index)
{
	this->m_enemyText.setPosition(this->enemies[index].getPosition().x,
		this->enemies[index].getPosition().y - 
		this->enemies[index].getGobalBounds().height);

	this->m_enemyText.setString(std::to_string(this->enemies[index].getHp()) + "/" +
		std::to_string(this->enemies[index].getHpMax()));
}

void Game::update(const float &deltaTime)
{
	this->updateTimers(deltaTime);

	this->toggleFullscreen();

	this->updateView();

	this->pauseGame();

	this->updateWhilePaused(deltaTime);
	
	if (this->m_playerAlive > 0 && !this->m_paused)
	{
		this->updateTimersUnpaused(deltaTime);

		this->updateDifficulty();

		this->updateScore();

		this->playerUpdate(deltaTime);

		this->enemyUpdate(deltaTime);

		this->textTagsUpdate(deltaTime);

		this->upgradesUpdates(deltaTime);

		this->pickupsUpdate(deltaTime);

		this->particlesUpdate(deltaTime);
	}

	else if (this->m_playerAlive <= 0 && this->m_scoreTime == 0)
	{
		this->setEndingScoreBoard();
	}

	if (this->m_playerAlive <= 0)
		this->restartUpdate();
}

void Game::mapUpdate()
{
}

void Game::draw()
{
	this->m_window->clear();

	//Background
	this->m_window->draw(m_sprite);

	//Set View
	//this->m_window->setView(this->m_mainView);

	//Map
	this->drawMap();

	//Draw Players
	this->drawPlayer();

	//Draw Enemies
	this->drawEnemies();

	//Draw Pickups
	this->drawPickups();

	//Draw Upgrades
	this->drawUpgrades();

	//Draw Particles
	this->drawParticles();

	//Set View
	this->m_window->setView(this->m_window->getDefaultView());

	//Draw UI
	this->drawUI();

	this->m_window->display();
}


void Game::combatUpdate()
{
	
}

void Game::updateView()
{
	this->m_mainView.setCenter(this->players[0].getPosition());
}

void Game::pauseGame()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && this->m_keyTime >= this->m_keyTimeMax)
	{
		if (this->m_paused)
			this->m_paused = false;
		else
			this->m_paused = true;

		this->m_keyTime = 0.f;
	}
}

void Game::updateScore()
{
	if (this->m_multiplierAdder >= this->m_multiplierAdderMax)
	{
		this->m_multiplierAdder = 0;
		this->m_scoreMultiplier++;
	}
}

void Game::initView()
{
	this->m_mainView.setSize(sf::Vector2f(this->m_window->getSize()));
	this->m_mainView.setCenter(sf::Vector2f(this->m_window->getSize().x / 2,
		this->m_window->getSize().y / 2));
}

void Game::restartUpdate()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
	{
		for (size_t i = 0; i < this->players.size(); i++)
		{
			this->players[i].reset();
		}

		this->m_playerAlive = this->players.size();
		this->m_score = 0;
		this->m_scoreMultiplier = 1;
		this->m_multiplierAdder = 0;
		this->m_scoreTime = 0;
		this->m_difficulty = 0;
		this->m_enemySpawnTimerMax = 35.f;
		this->m_scoreTimer.restart();
		this->enemies.clear();
		this->m_upgrades.clear();
		this->m_pickups.clear();
		this->m_bosses.clear();
		this->m_bossEncounter = false;
	}
}

void Game::toggleFullscreen()
{
	//Fullscreen
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1) && this->m_keyTime >= this->m_keyTimeMax)
	{
		this->m_keyTime = 0.f;

		if (m_fullscreen)
		{
			this->m_fullscreen = false;
			this->m_window->close();
			this->m_window->create(sf::VideoMode(1935, 1080), "2D Shooter", sf::Style::Fullscreen);
		}
		else
		{
			this->m_fullscreen = true;
			this->m_window->close();
			this->m_window->create(sf::VideoMode(1935, 1080), "2D Shooter", sf::Style::Close);
		}
	}
}

void Game::setEndingScoreBoard()
{
	this->m_scoreTime = (int)this->m_scoreTimer.getElapsedTime().asSeconds();

	if (this->m_scoreTime == 0)
		this->m_scoreTime = 1;

	this->m_endGameText.setString(std::string("Game the fuck Over!! \n Score: "
		+ std::to_string(this->m_score) + "\n" + "Time: " +
		std::to_string(this->m_scoreTime) + "\n " + "Score / Second " +
		std::to_string((int)round((double)this->m_score / (double)this->m_scoreTime))) +
		"\n F2 to Restart");

	if ((double)this->m_score / (double)this->m_scoreTime > this->m_bestScoreSecond)
		this->m_bestScoreSecond = (double)this->m_score / (double)this->m_scoreTime;
}

void Game::playerUpdate(const float & deltaTime)
{
	//Game Start
	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].isAlive())
		{
			//Update Players
			this->players[i].update(this->m_window->getSize(), deltaTime);
			this->players[i].auraChange();
			

			//Bullets Update
			this->playerBulletUpdate(deltaTime, i);

			//Update Score
			this->m_score = 0;

			for (size_t k = 0; k < this->players.size(); k++)
			{
				this->m_score += this->players[k].getScore();
			}

			this->m_scoreText.setString("Score:  " + std::to_string(this->m_score) +
				"\n Multiplier: " + std::to_string(this->m_scoreMultiplier) + " x " +
				"\n Multiplier Timer: " + std::to_string((int)this->m_multiplierTimer) +
				"\n New Multiplier: " + std::to_string(this->m_multiplierAdder) + "/" +
				std::to_string(this->m_multiplierAdderMax) + "\n Game Time: " +
				std::to_string((int)this->m_scoreTimer.getElapsedTime().asSeconds()) +
				"\n Difficulty " + std::to_string(this->m_difficulty) +
				"\n Best Score / Second: " + std::to_string(this->m_bestScoreSecond));
		}
	}
}


void Game::playerBulletUpdate(const float & deltaTime, const int i)
{
	//Bullets Update
	for (size_t k = 0; k < this->players[i].getBulletSize(); k++)
	{
		this->players[i].getBullet(k).update(deltaTime);

		//Collision check motherfucker
		for (size_t j = 0; j < this->enemies.size(); j++)
		{
			if (this->players[i].getBullet(k).getGobalBounds().intersects(this->enemies[j].getGobalBounds()))
			{
				int nrOfParticles = rand() % 5 + 10;

				for (size_t l = 0; l < nrOfParticles; l++)
				{
					this->m_particles.add(Particles(this->players[i].getBullet(k).getPosition(), 0,
						this->players[i].getBullet(k).getVelocity(), 
						rand() % 30 + 10, rand() % 15, 100.f));
				}

				//Piercing shot check / remove bullet
				if (!this->players[i].getPiercingShot())
					this->players[i].removeBullet(k);

				/*int frozen = rand() % 10;

				if (this->m_isFrozen)
				{
					if (frozen <= 5)
					{
						this->enemies[j].stopMovement();
					}
				}*/

				//Enemy Take Damage
				int damage = this->players[i].getDamage();
				if (this->enemies[j].getHp() > 0)
				{
					this->enemies[j].takeDamage(damage);

					//Enemy Damage
					this->m_textTags.add(TextTag(&this->m_font, "-" + std::to_string(damage),
						sf::Color(110, 200, 200, 255),
						sf::Vector2f(this->enemies[j].getPosition().x + 20.f,
							this->enemies[j].getPosition().y - 20.f), sf::Vector2f(-1.f, 0.f), 40, 30.f, true));

					this->m_textTags.add(TextTag(&this->m_font, "BOOM!!",
						sf::Color(220, 20, 60, 255),
						sf::Vector2f(this->enemies[j].getPosition().x + 20.f,
							this->enemies[j].getPosition().y + 20.f), sf::Vector2f(1.5f, 0.f), 40, 13.2f, true));
				}

				//Enemy Dead 
				if (this->enemies[j].getHp() <= 0)
				{
					//Gain Score
					this->m_multiplierTimer = this->m_multiplierTimerMax;
					int score = this->enemies[j].getHpMax() * this->m_scoreMultiplier;
					this->m_multiplierAdder++;
					this->players[i].gainScore(score);

					//Gain EXP
					int exp = this->enemies[j].getHpMax()
						+ (rand() % this->enemies[j].getHpMax() + 1)
						* this->m_scoreMultiplier;

					//Score TextTag
					this->m_textTags.add(TextTag(&this->m_font, "+" + std::to_string(score) +
						"  (  " + "x " + std::to_string(this->m_scoreMultiplier) + "  )",
						sf::Color(0, 100, 0, 200), sf::Vector2f(50.f, 150.f),
						sf::Vector2f(1.f, 0.f), 40, 50.f, true));

					if (this->players[i].gainExp(exp))
					{
						//Level Up TextTag
						this->m_textTags.add(TextTag(&this->m_font, "+" + std::to_string(this->m_scoreMultiplier)
							+ "(  x" + ")" + "  EXP!!",
							sf::Color::Red, sf::Vector2f(this->players[i].getPosition().x + 20.f,
								this->players[i].getPosition().y - 20.f), sf::Vector2f(0.f, 1.f), 40, 30.2f, true));
					}

					//Gain Exp TextTag
					this->m_textTags.add(TextTag(&this->m_font, "+" + std::to_string(exp) + " exp", sf::Color::Yellow,
						sf::Vector2f(this->players[i].getPosition().x + 20.f,
							this->players[i].getPosition().y - 20.f), sf::Vector2f(0.f, 1.f), 30, 25.6f, true));


					//Add Pickup
					int dropChance = rand() % 100 + 1;
					int uType = 0;

					if (dropChance > 70)
					{
						dropChance = rand() % 100 + 1;

						if (dropChance > 10)
						{
							this->m_pickups.add(Pickup(this->m_pickupTextures,
								this->enemies[j].getPosition(), 0, 150.f));
						}
					}

					else
					{
						//Add Upgrades
						int dropChance = rand() % 100 + 1;
						if (dropChance > 70)
						{
							uType = rand() % this->m_nrOfUpgrades;

							for (size_t k = 0; k < this->players[i].getAquiredUpgrades().size(); k++)
							{
								if (uType == this->players[i].getAquiredUpgrades()[k])
								{
									uType = rand() % 10;
								}
							}

							this->m_upgrades.add(Upgrades(this->m_upgradeTextures, this->enemies[j].getPosition(),
								uType, 500.f));
						}
					}

					this->enemies.remove(j);
				}
				return;
			}
		}

		//Window Bounds
		if (this->players[i].getBullet(k).getPosition().x > this->m_window->getSize().x)
		{
			this->players[i].removeBullet(k);

			return;
		}
	}
}

void Game::enemyUpdate(const float & deltaTime)
{
	//Spawn Enemies
	if (this->m_enemySpawnTimer >= this->m_enemySpawnTimerMax)
	{
		this->enemies.add(Enemy(this->m_enemyTextures, this->m_enemyBulletTextures, this->m_window->getSize(),
			sf::Vector2f(0.f, 0.f), sf::Vector2f(-1.f, 0.f), rand() % 3, this->players[(rand() %
				this->m_playerAlive)].getLevel(), rand() % this->m_playerAlive));

		//Reset Timer
		this->m_enemySpawnTimer = 0;
	}

	//Update Enemies
	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].update(deltaTime, this->players[this->enemies[i].getPlayerFollowNr()].getPosition());

		//Enemy / Player Collision
		for (size_t k = 0; k < this->players.size(); k++)
		{
			if (this->players[k].isAlive())
			{
				if (this->players[k].getGobalBounds().intersects(this->enemies[i].getGobalBounds())
					&& !this->players[k].isDamageCoolDown())
				{
					//Enemy / Player Collision TextTag
					int damage = this->enemies[i].getDamage();
					this->players[k].takeDamage(this->enemies[i].getDamage());

					this->enemies[i].collision();

					this->m_textTags.add(TextTag(&this->m_font, "-" + std::to_string(damage),
						sf::Color(190, 100, 200, 255),
						sf::Vector2f(this->players[k].getPosition().x + 20.f,
							this->players[k].getPosition().y - 20.f), sf::Vector2f(-1.f, 0.f), 40, 15.f, true));

					this->m_textTags.add(TextTag(&this->m_font, "Ouch!",
						sf::Color(255, 165, 0, 255),
						sf::Vector2f(this->players[k].getPosition().x + 20.f,
							this->players[k].getPosition().y - 20.f), sf::Vector2f(-3.5f, 4.f), 40, 15.f, false));

					//Player Death
					if (!this->players[k].isAlive())
						this->m_playerAlive--;

					return;
				}
			}
		}

		//Enemies out of Bounds
		if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGobalBounds().width)
		{
			this->enemies.remove(i);
			return;
		}
	}

	//Enemy Bullet Update
	for (size_t k = 0; k < Enemy::enemyBullets.size(); k++)
	{
		Enemy::enemyBullets[k].update(deltaTime);

		for (size_t i = 0; i < this->players.size(); i++)
		{
			//Player Collision Check Motherfucker
			if (Enemy::enemyBullets[k].getGobalBounds().intersects(this->players[i].getGobalBounds()))
			{
				//Enemy / Player Bullet Collision TextTag
				int damage = this->enemies[i].getDamage();

				if (!this->players[k].isShielding())
				{
					this->players[k].takeDamage(this->enemies[i].getDamage());

					this->m_textTags.add(TextTag(&this->m_font, "-" + std::to_string(damage),
						sf::Color(190, 100, 200, 255),
						sf::Vector2f(this->players[k].getPosition().x + 20.f,
							this->players[k].getPosition().y - 20.f), sf::Vector2f(-1.f, 0.f), 40, 15.f, true));

					this->m_textTags.add(TextTag(&this->m_font2, "FUCK!",
						sf::Color(255, 165, 0, 255),
						sf::Vector2f(this->players[k].getPosition().x + 20.f,
							this->players[k].getPosition().y - 20.f), sf::Vector2f(-3.5f, 4.f), 40, 15.f, false));

					//Player Death
					if (!this->players[k].isAlive())
						this->m_playerAlive--;

				}
				else
				{
					//Player Shield Tag
					this->m_textTags.add(TextTag(&this->m_font, "-" + std::to_string(0),
						sf::Color(128, 0, 128, 205),
						sf::Vector2f(this->players[k].getPosition().x + 20.f,
							this->players[k].getPosition().y - 20.f), sf::Vector2f(-1.f, 0.f), 40, 15.f, true));

				}
				Enemy::enemyBullets.remove(k);
				return;
			}
		}

		//Window Bounds
		if (Enemy::enemyBullets[k].getPosition().x > this->m_window->getSize().x
			|| Enemy::enemyBullets[k].getPosition().x < 0 ||
			Enemy::enemyBullets[k].getPosition().y > this->m_window->getSize().y
			|| Enemy::enemyBullets[k].getPosition().y < 0)
		{
			Enemy::enemyBullets.remove(k);

			return;
		}
	}

	//Text Tags
	for (size_t i = 0; i < this->m_textTags.size(); i++)
	{
		this->m_textTags[i].update(deltaTime);

		if (this->m_textTags[i].getTimer() <= 0.f)
		{
			this->m_textTags.remove(i);

			break;
		}
	}
}

void Game::pickupsUpdate(const float & deltaTime)
{
	//Pickups update
	for (size_t i = 0; i < this->m_pickups.size(); i++)
	{
		this->m_pickups[i].update(deltaTime);

		for (size_t k = 0; k < this->players.size(); k++)
		{
			if (this->m_pickups[i].checkCollision(this->players[k].getGobalBounds()))
			{
				int gainHp = this->players[k].getHpMax() / 5;

				switch (this->m_pickups[i].getType())
				{
				case 0: //HP
					if (this->players[k].getHp() < this->players[k].getHpMax())
					{
						this->players[k].gainHp(gainHp);

						//Gain Hp Tag
						this->m_textTags.add(TextTag(&this->m_font1, "+ " +
							std::to_string(gainHp) + " HP", sf::Color(0, 99, 128, 250),
							sf::Vector2f(this->players[k].getPosition()),
							sf::Vector2f(0.f, -1.f), 24, 40.f, true));
					}

					else
					{
						//Gain Hp Tag
						this->m_textTags.add(TextTag(&this->m_font, "+" +
							std::to_string(10) + " EXP", sf::Color(0, 255, 255, 255),
							this->m_pickups[i].getPosition(), sf::Vector2f(1.f, 0.f),
							24, 40.f, true));

						if (this->players[k].gainExp(10))
						{
							this->m_textTags.add(TextTag(&this->m_font2, "LEVEL UP!!!",
								sf::Color::White, sf::Vector2f(this->players[i].getPosition().x, this->players[i].getPosition().y),
								sf::Vector2f(0.f, 1.f), 32, 40.f, true));
						}
					}
					break;
				case 1: //Missile
					break;
				case 2: //Missile
					break;
				default:
					break;
				}

				this->m_pickups.remove(i);
				return;
			}
		}
		if (this->m_pickups[i].canDelete())
		{
			this->m_pickups.remove(i);
		}
	}

}

void Game::particlesUpdate(const float & deltaTime)
{
	for (size_t i = 0; i < this->m_particles.size(); i++)
	{
		this->m_particles[i].update(deltaTime);
	}
}

void Game::upgradesUpdates(const float & deltaTime)
{
	//Update upgrades
	for (size_t j = 0; j < this->m_upgrades.size(); j++)
	{
		this->m_upgrades[j].update(deltaTime);

		for (size_t t = 0; t < this->players.size(); t++)
		{
			if (this->m_upgrades[j].checkCollision(this->players[t].getGobalBounds()))
			{
				if (this->m_upgrades[j].getType() != 0 && this->m_upgrades[j].getType() != 1)
					this->players[t].getAquiredUpgrades().add(this->m_upgrades[j].getType());

				switch (this->m_upgrades[j].getType())
				{
				case 0:
					this->players[t].addStatPointRandom();

					this->m_textTags.add(TextTag(&this->m_font, "Random Stat Point Upgrade",
						sf::Color::Yellow, this->players[t].getPosition(),
						sf::Vector2f(1.f, 0.f), 40, 100.f, true));

					break;

				case 1: //Healthtank
					this->players[t].upgradeHp();

					this->m_textTags.add(TextTag(&this->m_font2, "Health Upgrade",
						sf::Color::Yellow, this->players[t].getPosition(),
						sf::Vector2f(1.f, 0.f), 40, 100.f, true));

					break;

				case 2: //Doubleray
					//this->players[t].disableIceRay();
					//this->players[t].setCurrentWeapon();
					//this->m_isFrozen = false; 
					
					if (this->players[t].getGunLevel() >= 0)
						this->players[t].setGunLevel(1);

					this->m_textTags.add(TextTag(&this->m_font2, "Motherfucking Double Ray!!",
						sf::Color::Yellow, this->players[t].getPosition(),
						sf::Vector2f(1.f, 0.f), 40, 100.f, true));

					break;

				case 3: //Tripleray
					//this->players[t].disableIceRay();
					//this->players[t].setCurrentWeapon();
					//this->m_isFrozen = false;
					
					if (this->players[t].getGunLevel() <= 1)
						this->players[t].setGunLevel(2);

					this->m_textTags.add(TextTag(&this->m_font2, "Oh shit son, Triple Ray!!",
						sf::Color::Yellow, this->players[t].getPosition(),
						sf::Vector2f(1.f, 0.f), 40, 100.f, true));

					break;

				case 4: //Piercing
					this->players[t].enablePierchingShot();

					this->m_textTags.add(TextTag(&this->m_font2, "Piercing Shot",
						sf::Color::Yellow, this->players[t].getPosition(),
						sf::Vector2f(1.f, 0.f), 40, 100.f, true));

					break;

				case 5: //Shield
					this->players[t].enableShield();

					this->m_textTags.add(TextTag(&this->m_font2, "Shield",
						sf::Color::Yellow, this->players[t].getPosition(),
						sf::Vector2f(1.f, 0.f), 40, 100.f, true));
					break;

				case 6: //Iceray
						/*this->players[t].enableIceRay();
						this->players[t].disablePierchingShot();
						this->players[t].setCurrentWeapon();

						this->m_isFrozen = true;*/

					this->m_textTags.add(TextTag(&this->m_font2, "It just got cold, Ice Ray enabled",
						sf::Color(0, 191, 255, 255), this->players[t].getPosition(),
						sf::Vector2f(1.f, 0.f), 40, 100.f, true));

					break;

				default:
					break;
				}
				this->m_upgrades.remove(j);
				return;
			}
		}
		if (this->m_upgrades[j].canDelete())
		{
			this->m_upgrades.remove(j);
		}
	}
}

void Game::initTextures()
{
	sf::Texture temp;

	//Map
	this->initMapTextures();

	//Player
	this->initPlayerTextures();
	
	//Pickup Textures
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Pickups\\hpSupply.png");
	this->m_pickupTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Pickups\\missileHSupply.png");
	this->m_pickupTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Pickups\\missileSupply.png");
	this->m_pickupTextures.add(sf::Texture(temp));
	this->m_nrOfPickUps = this->m_pickupTextures.size();

	//Upgrades 
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Upgrades\\statpoint.png");
	this->m_upgradeTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Upgrades\\healthtank.png");
	this->m_upgradeTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Upgrades\\doubleray.png");
	this->m_upgradeTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Upgrades\\tripleray.png");
	this->m_upgradeTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Upgrades\\piercingshot.png");
	this->m_upgradeTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Upgrades\\shield.png");
	this->m_upgradeTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Upgrades\\iceray.png");
	this->m_upgradeTextures.add(sf::Texture(temp));
	this->m_nrOfUpgrades = this->m_upgradeTextures.size();
	
	//Enemies Textures
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\enemyMoveLeft.png");
	this->m_enemyTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\enemyFollow.png");
	this->m_enemyTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\enemyMoveLeftShoot.png");
	this->m_enemyTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Guns\\roundBulletRed.png");
	this->m_enemyBulletTextures.add(sf::Texture(temp));

	//Boss textures
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Bosses\\Bodies\\BossBody01.png");
	this->m_bossBodyTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Bosses\\Guns\\BossGun01.png");
	this->m_bossGunTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Bosses\\Bullets\\BossBullet02.png");
	this->m_bossBulletTextures.add(sf::Texture(temp));

	//Particles
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Particles\\particle01.png");
	Particles::particleTextures.add(temp);
}

void Game::initUI()
{
	//Follow Text
	this->m_followPlayerTexts.setFont(this->m_font);
	this->m_followPlayerTexts.setCharacterSize(14);
	this->m_followPlayerTexts.setStyle(sf::Text::Italic | sf::Text::Bold);
	this->m_followPlayerTexts.setFillColor(sf::Color(252, 247, 94, 255));
	
	//Static Text
	this->m_staticPlayerTexts.setFont(this->m_font);
	this->m_staticPlayerTexts.setCharacterSize(14);
	this->m_staticPlayerTexts.setFillColor(sf::Color::Yellow);
	this->m_staticPlayerTexts.setString("");

	//Exp Bar
	this->m_playerExpBar.setSize(sf::Vector2f(110.f, 10.f));
	this->m_playerExpBar.setFillColor(sf::Color(255, 165, 0, 255));

	//Shield Bar
	this->m_playerShieldBar.setSize(sf::Vector2f(110.f, 10.f));
	this->m_playerShieldBar.setFillColor(sf::Color(128, 0, 0, 255));
	
	//Enemy Texts
	this->m_enemyText.setFont(this->m_font);
	this->m_enemyText.setCharacterSize(16);
	this->m_enemyText.setStyle(sf::Text::Italic | sf::Text::Bold);
	this->m_enemyText.setFillColor(sf::Color::White);

	//End Game Stuff
	this->m_endGameText.setFont(this->m_font2);
	this->m_endGameText.setCharacterSize(64);
	this->m_endGameText.setStyle(sf::Text::Bold);
	this->m_endGameText.setFillColor(sf::Color(0, 255, 255, 255));
	this->m_endGameText.setPosition((this->m_window->getSize().x / 4) - 100.f, (this->m_window->getSize().y /2) - 400);
	
	//Exit Game Stuff
	this->m_exitGameText.setFont(this->m_font2);
	this->m_exitGameText.setCharacterSize(56);
	this->m_exitGameText.setStyle(sf::Text::Bold);
	this->m_exitGameText.setFillColor(sf::Color(0, 255, 255, 255));
	this->m_exitGameText.setString("Press ESC to exit");
	this->m_exitGameText.setPosition((this->m_window->getSize().x / 4) -100.f, (this->m_window->getSize().y / 2) + 60);

	//SCORE!!!!
	this->m_scoreText.setFont(this->m_font);
	this->m_scoreText.setCharacterSize(25);
	this->m_scoreText.setStyle(sf::Text::Bold);
	this->m_scoreText.setFillColor(sf::Color(173, 255, 47, 255));
	this->m_scoreText.setString("Score 0");
	this->m_scoreText.setPosition(10.f, 0.f);

	//Controls Text
	this->m_controlsTexts.setFont(this->m_font);
	this->m_controlsTexts.setCharacterSize(25);
	this->m_controlsTexts.setStyle(sf::Text::Bold);
	this->m_controlsTexts.setFillColor(sf::Color(255, 215, 0, 255));
	this->m_controlsTexts.setString("A: LEFT \n D: RIGHT \n W: UP \n S: DOWN \n SHOOT: SPACEBAR \n");
	this->m_controlsTexts.setPosition(50.f, 400.f);

	//Stats Text
	this->m_playerStatsTexts.setFont(m_font);
	this->m_playerStatsTexts.setFillColor(sf::Color(189, 183, 107, 255));
	this->m_playerStatsTexts.setCharacterSize(20);
	this->m_playerStatsTexts.setString("None");
}

void Game::initMap()
{
	
}

void Game::drawParticles()
{
	for (size_t i = 0; i < this->m_particles.size(); i++)
	{
		this->m_particles[i].draw(*this->m_window);
	}
}

void Game::drawUI()
{
	for (size_t j = 0; j < this->m_textTags.size(); j++)
	{
		this->m_textTags[j].draw(*this->m_window);
	}

	//Game Over Text
	if (this->m_playerAlive <= 0)
	{
		this->m_window->draw(this->m_endGameText);
		this->m_window->draw(this->m_exitGameText);
	}

	//Score Text
	this->m_window->draw(this->m_scoreText);

	//Controls Text
	if (this->m_paused == true)
		this->m_window->draw(this->m_controlsText);
}

void Game::drawPlayer()
{
	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].isAlive())
		{
			this->players[i].draw(*this->m_window);

			this->updateUIPlayer(i);

			this->m_window->draw(this->m_followPlayerTexts); //Player UI

			this->m_window->draw(this->m_playerExpBar);

			this->m_window->draw(this->m_playerShieldBar);

			if (this->players[i].playerShowStatsIsPressed())
			{
				this->m_window->draw(this->m_playerStatsTextsBackground);
				this->m_window->draw(this->m_playerStatsTexts);
			}
		}
	}
}

void Game::drawEnemies()
{
	for (size_t j = 0; j < enemies.size(); j++)
	{
		this->enemies[j].draw(*this->m_window);

		this->updateUIEnemy(j);

		this->m_window->draw(this->m_enemyText);
	}

	for (size_t i = 0; i < Enemy::enemyBullets.size(); i++)
	{
		Enemy::enemyBullets[i].draw(*this->m_window);
	}
}

void Game::drawMap()
{
	this->m_stage.draw(*this->m_window, this->m_mainView);
}

void Game::drawPickups()
{
	//Pickups
	for (size_t k = 0; k < this->m_pickups.size(); k++)
	{
		this->m_pickups[k].draw(*this->m_window);
	}
}

void Game::drawUpgrades()
{
	//Upgrades
	for (int h = 0; h < this->m_upgrades.size(); h++)
	{
		this->m_upgrades[h].draw(*this->m_window);
	}
}

void Game::initialize()
{
	//Init View
	this->initView();

	//Init / Set Background
	this->m_backgroundTexture.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Backgrounds\\spacegen_01.png");

	//Init Fonts
	this->m_font.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Fonts\\Anton-Regular.ttf");
	this->m_font1.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Fonts\\MetalMacabre.ttf");
	this->m_font2.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Fonts\\PermanentMarker.ttf");

	//Init textures
	this->initTextures();

	//Init Player
	this->players.add(Player());

	/*this->players.add(Player(
		sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, 
		sf::Keyboard::Right, sf::Keyboard::LControl));*/

	this->m_playerAlive = this->players.size();

	this->m_enemySpawnTimerMax = 35.f;
	this->m_enemySpawnTimer = this->m_enemySpawnTimerMax;

	this->m_backgroundTexture.setSmooth(true);
	this->m_sprite.setTexture(this->m_backgroundTexture);

	this->initUI();

	this->initMap();

	//Init Bosses
	this->m_bossEncounter = false;

	this->m_isFrozen = false;

}

void Game::initPlayerTextures()
{
	sf::Texture temp;
	//Init Player Textures
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\ship.png");
	Player::playerBodyTextures.add(temp);

	//Bullet Textures
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Guns\\rayTex01.png");
	Player::playerBulletTextures.add(temp);
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Guns\\missileTex01.png");
	Player::playerBulletTextures.add(temp);
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Guns\\icerayTex01.png");
	Player::playerIceBulletTextures.add(temp);

	//Gun Textures
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Guns\\gun01.png");
	Player::playerMainGunTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Guns\\gun02.png");
	Player::playerMainGunTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Guns\\gun03.png");
	Player::playerMainGunTextures.add(sf::Texture(temp));
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Guns\\iceray.png");
	Player::playerMainGunIceRayTextures.add(sf::Texture(temp));
	

	//Player Shields
	temp.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Player\\shield.png");
	Player::playerShieldTextures.add(sf::Texture(temp));

	//Accessorie Textures
	std::ifstream in;

	in.open("D:\\VS Projects\\2D Shooter\\res\\Textures\\Accessories\\leftwings1.txt");
	std::string fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			sf::Texture temp;
			temp.loadFromFile(fileName);
			Player::lWingTextures.add(sf::Texture(temp));
		}
	}

	in.close();

	in.open("D:\\VS Projects\\2D Shooter\\res\\Textures\\Accessories\\rightwings.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			sf::Texture temp;
			temp.loadFromFile(fileName);
			Player::rWingTextures.add(sf::Texture(temp));
		}
	}

	in.close();

	in.open("D:\\VS Projects\\2D Shooter\\res\\Textures\\Accessories\\cockpits.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			sf::Texture temp;
			temp.loadFromFile(fileName);
			Player::cPitTextures.add(sf::Texture(temp));
		}
	}

	in.close();

	in.open("D:\\VS Projects\\2D Shooter\\res\\Textures\\Accessories\\auras.txt");
	fileName = "";

	if (in.is_open())
	{
		while (getline(in, fileName))
		{
			sf::Texture temp;
			temp.loadFromFile(fileName);
			Player::auraTextures.add(sf::Texture(temp));
		}
	}

	in.close();
}

void Game::textTagsUpdate(const float & deltaTime)
{
	for (size_t i = 0; i < this->m_textTags.size(); i++)
	{
		this->m_textTags[i].update(deltaTime);

		if (this->m_textTags[i].getTimer() <= 0.f)
		{
			this->m_textTags.remove(i);
			break;
		}
	}
}

void Game::initMapTextures()
{
	Tile::tileTextures.loadFromFile("D:\\VS Projects\\2D Shooter\\res\\Textures\\Map\\textureSheet.png");
}





