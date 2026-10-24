#pragma once
#include "HumenPlayerController.h"
#include "AIManager.h"

class EventBus;
class GameState;

namespace sf {
	class Event;
}	

class GameController {
public:
	GameController(
		GameState& state,
		EventBus& events
	) : m_mState(state), m_mModelEvents(events) {};

public:
	void InitializeOnline();
	void InitializeOffline();

public:
	void UpdateOnline(float dt);
	void UpdateOffline(float dt);

public:
	bool HandleEvent(const sf::Event& e);

private:
	EventBus& m_mModelEvents;
	GameState& m_mState;

private:
	std::unique_ptr<AIManager> m_mAIManager;
	std::vector<std::unique_ptr<Command>> m_mCommand;
};