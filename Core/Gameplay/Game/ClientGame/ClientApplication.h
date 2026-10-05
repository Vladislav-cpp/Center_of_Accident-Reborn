#pragma once

#include <SFML/Graphics.hpp>

#include "EventBus.h"
#include "GameSession.h"

class GameController;
class ClientNetworkSystem;
class Render;
class ScreenManager;

enum class ApplicationMode {
	Interface,
	OnlineGame,
	OfflineGame
};

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

	void OnOnlineGameRequested();
	void OnOfflineGameRequested();
	void OnReturnToInterfaceRequested();

	void OnConnectionStateChanged(
		//const ConnectionStateChangedEvent& event
	);

private:
	ApplicationMode			m_mMode		= ApplicationMode::Interface;

	EventBus*				m_mEvents	= nullptr;

	sf::RenderWindow*		m_mWindow	= nullptr;
	GameSession*			m_mSession	= nullptr;

	ClientNetworkSystem*	m_mNetwork	= nullptr;
	Render*					m_mRender	= nullptr;
	ScreenManager*			m_mScreens	= nullptr;

	GameController*			m_mGame		= nullptr;
};