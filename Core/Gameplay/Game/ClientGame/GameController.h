#pragma once

class EventBus;
class ClientNetworkSystem;
class GameSession;
class Render;

class GameController
{
public:
	GameController(
		EventBus& events,
		ClientNetworkSystem& network,
		GameSession& session
	);

	void UpdateOnline(float dt);
	void UpdateOffline(float dt);

	void RenderWorld(Render& render, float dt);

private:
	void SendPlayerActivities(float dt);

	void OnConnectionStateChanged(
		//const ConnectionStateChangedEvent& event
	);

	void CreateLocalPlayer();

private:
	EventBus& m_mEvents;
	ClientNetworkSystem& m_mNetwork;
	GameSession& m_mSession;
};