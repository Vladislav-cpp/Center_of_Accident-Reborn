#pragma once

#include <SFML/Graphics.hpp>

class EventBus;
class GameController;
class GameState;
class ClientNetworkSystem;
class Render;
class ScreenManager;

class ClientApplication {
public:
	ClientApplication();
	~ClientApplication();

	void Run();

private:
	void Initialize();

	void ProcessEvents();

	void Update(float dt);
	void UpdateInterface(float dt);
	void UpdateOnlineGame(float dt);
	void UpdateOfflineGame(float dt);

	void Draw(float dt);

	void SubscribeToEvents();

private:
	// Game Info/state
	GameState*				m_mState	= nullptr;

	EventBus*				m_mEvents	= nullptr;

	sf::RenderWindow*		m_mWindow	= nullptr;

	ClientNetworkSystem*	m_mNetwork	= nullptr;
	Render*					m_mRender	= nullptr;
	ScreenManager*			m_mScreens	= nullptr;

	GameController*			m_mGame		= nullptr;
};