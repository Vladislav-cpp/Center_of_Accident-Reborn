#pragma once

class EventBus;
class ClientNetworkSystem;
class Render;

class GameController {
public:
	GameController(
		EventBus& events
	) : m_mModelEvents(events) {};

public:
	void UpdateOnline(float dt);
	void UpdateOffline(float dt);

	void RenderWorld(Render& render, float dt);


private:
	void SendPlayerActivities(float dt);

	void CreateLocalPlayer();

private:
	EventBus& m_mModelEvents;
};