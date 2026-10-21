#pragma once

#include <SFML/Graphics.hpp>
#include <ClientEvents.h>

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
	void OnOnlineGame(const OnlineGameRequestedEvent& event);
	void OnOfflineGame(const OfflineGameRequestedEvent& event);
	void OnReturnToInterface(const ReturnToInterfaceRequestedEvent& event);
	void OnConnectionStateChanged(const ConnectionStateChangedEvent& event);

private:
	// Game Info/state
	GameState*				m_mState		= nullptr;
	EventBus*				m_mModelEvents	= nullptr;
	ClientNetworkSystem*	m_mNetwork		= nullptr;
	ScreenManager*			m_mScreens		= nullptr;
	GameController*			m_mGame			= nullptr;

	sf::RenderWindow*		m_mWindow		= nullptr;
	Render*					m_mRender		= nullptr;
};