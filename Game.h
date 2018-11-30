#pragma once
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

class Game
{
public:
	Game(sf::RenderWindow* window);
	virtual ~Game();
	
	//Accessors
	inline sf::RenderWindow& getWindow() { return *this->m_window; }

	//Setters

	//Functions
	void updateView();
	void pauseGame();
	void updateScore();
	void initView();
	void restartUpdate();
	void toggleFullscreen();
	void setEndingScoreBoard();
	void playerUpdate(const float &deltaTime);
	void playerBulletUpdate(const float &deltaTime, const int i);
	void enemyUpdate(const float &deltaTime);
	void pickupsUpdate(const float &deltaTime);
	void particlesUpdate(const float &deltaTime);
	void upgradesUpdates(const float &deltaTime);
	void updateUIPlayer(int index);
	void updateTimers(const float &deltaTime);
	void updateTimersUnpaused(const float &deltaTime);
	void updateWhilePaused(const float &deltaTime);
	void updateDifficulty();
	void updateUIEnemy(int index);
	void update(const float &deltaTime);
	void mapUpdate();
	void draw();
	void combatUpdate();
	void initTextures();
	void initUI();
	void initMap();
	void drawParticles();
	void drawUI();
	void drawPlayer();
	void drawEnemies();
	void drawMap();
	void drawPickups();
	void drawUpgrades();
	void initialize();
	void initPlayerTextures();
	void textTagsUpdate(const float &deltaTime);
	void initMapTextures();
	
private:
	sf::RenderWindow* m_window;
	sf::Color m_color;
	sf::View m_mainView; 

	float m_keyTime;
	float m_keyTimeMax;
	bool m_fullscreen;
	bool m_paused;

	//Score
	unsigned m_score;
	unsigned m_scoreMultiplier; 
	sf::Clock m_scoreTimer;
	int m_scoreTime;
	float m_multiplierTimer;
	float m_multiplierTimerMax;
	int m_multiplierAdder;
	int m_multiplierAdderMax;
	double m_bestScoreSecond;
	float m_difficultyTimer;
	int m_difficulty;

	float m_deltaTimeMultiplier; 
	
	sf::Sprite m_sprite;

	//Fonts
	sf::Font m_font;
	sf::Font m_font1;
	sf::Font m_font2; 

	//Texts
	sf::Text m_followPlayerTexts;
	sf::Text m_staticPlayerTexts; 
	sf::Text m_enemyText; 
	sf::Text m_endGameText; 
	sf::Text m_exitGameText;
	sf::Text m_scoreText; 
	sf::Text m_finalScoreText;
	sf::Text m_finalScoreText1;
	sf::Text m_controlsText;
	sf::Text m_playerStatsTexts;
	sf::Text m_controlsTexts;

	//Bars
	sf::RectangleShape m_playerExpBar; 
	sf::RectangleShape m_playerShieldBar;

	//Texts Background
	sf::RectangleShape m_playerStatsTextsBackground;

	//Map
	Map m_map;
	dArr<Tile> m_tiles;
	Stage m_stage; 

	//TextTags
	dArr<TextTag> m_textTags; 

	//Pickups
	dArr<Pickup> m_pickups;

	//Upgrades
	dArr<Upgrades> m_upgrades;

	//Textures
	std::vector<sf::Texture> m_textures;
	sf::Texture m_backgroundTexture; 
	
	//Players
	dArr<Player> players;
	int m_playerAlive;

	//Pickups / Upgrades Textures
	unsigned m_nrOfUpgrades;
	unsigned m_nrOfPickUps;
	dArr<sf::Texture> m_pickupTextures;
	dArr<sf::Texture> m_upgradeTextures;

	//Particles
	dArr<Particles> m_particles; 
	
	//Enemies 
	dArr<sf::Texture> m_enemyTextures;
	dArr<sf::Texture> m_enemyBulletTextures;
	dArr<Enemy> enemies;
	dArr<Enemy> enemiesSaved; 
	bool m_isFrozen; 

	//Boss Textures
	dArr<sf::Texture> m_bossBodyTextures;
	dArr<sf::Texture> m_bossGunTextures;
	dArr<sf::Texture> m_bossBulletTextures;

	//Bosses
	dArr<Boss> m_bosses;
	bool m_bossEncounter;

	//Timers
	float m_enemySpawnTimer;
	float m_enemySpawnTimerMax;
};

