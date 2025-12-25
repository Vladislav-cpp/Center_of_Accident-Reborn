#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "GameSession.h"

class ClientNetworkSystem;
class Render;
class PlayerCharacter;
class ScreenManager;

class ClientGame {
	public:
	ClientGame();
	~ClientGame();

	public:
	void Run();

	private:
	void GameSessionRun(float dt);
	void ResurrectPlayer();
	private:
	void SendPlayerActivities(float dt);

	private:
	void ConectToServer();

	private:
	bool bGameSessionRunning = false;

	private:
	ClientNetworkSystem* m_xNetwork;
	sf::RenderWindow* m_xWindow;
	Render* m_xRender;
	ScreenManager* m_xScreens;

	private:
	GameSession m_xSession;
};