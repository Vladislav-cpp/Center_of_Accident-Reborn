#pragma once
#include <list>
#include <cmath>
#include <ctime>
#include <SFML/Audio.hpp>
#include "MapHUD.h"
#include "ImplSingleton.h"
#include "StaticObject.h"
#include "Utility.h"

class ActionHandler;
class PlayerCharacter;
class AICharacter;
class Projectile;
class Server;
class Client;

enum class GameMode {
	singl,
	server,
	client,
};

class Game : public ImplSingleton<Game> {
	friend class ImplSingleton<Game>;
	Game();

	public:
	GameMode mode = GameMode::singl;
	Server* m_pServer;
	Client* m_pClient;

	void RunSingle();
	void ClientSynchronization();

	void RunServer();
	void RunClient();

	virtual void Run();

	std::int32_t GetTime();
	std::int32_t GetRechargeTime();
	sf::Vector2f GetMousePosition();

	std::shared_ptr<PlayerCharacter> GetPlayer();

	void ClockRechargeRestart();

	void RegisterObject(std::shared_ptr<StaticObject> ob);

	private:
	void ClockRestart();
	void Menu_overview();

	void Set_coord_viev(float x, float y) {view.setCenter({x, y});}
	void spawn_bots(uint32_t id = 0);
	void updataCommands();
	void collision();
	void ServerCollision();
	void SyncWorldState();
	private:

	void drawWorld();

	public:
	void createPlayer(int idController, int id = 0);
	void createMainPlayer(int id = 0);

	utility::ObjectPool<std::shared_ptr<Projectile>> m_vProjectiles;
	std::shared_ptr<PlayerCharacter> player;

	private:

	//std::int32_t tempTime = 0;
	float actualTime = 0;
	ActionHandler* action;

	std::shared_ptr<PlayerCharacter> randServerPlayer;

	utility::ObjectPool<std::shared_ptr<AICharacter>> m_vAiPlayers;
	utility::ObjectPool<std::shared_ptr<StaticObject>> m_vAllObjects;
	utility::ObjectPool<std::shared_ptr<PlayerCharacter>> m_vAllPlayers;


	sf::View view;

	sf::Texture Tbackground;//-
	sf::Sprite Sprite_Tbackground;//-

	sf::Clock clock;
	sf::Clock clock_recharge;
	sf::Clock clock_create_opponents;
	sf::Cursor cursor;

	sf::SoundBuffer shotBuffer;//-	
	sf::Sound shootSound;//-

  
	sf::SoundBuffer explosionBuffer;//-
	sf::Sound explosion;//-
	sf::Music music;//-

	sf::RenderWindow window;

	MapHUD* Iw = nullptr;
};




