#pragma once
#include "HumenPlayerController.h"
#include "AIManager.h"

class EventBus;
class GameState;
class InputState;
class ClientNetworkSystem;

namespace sf {
	class Event;
}	

class GameController {
public:
	GameController(
		GameState& state,
		EventBus& events,
		ClientNetworkSystem& net
	) : m_mState(state), m_mModelEvents(events), m_mNetwork(net) {};

public:
	void InitializeOnline();
	void InitializeOffline();

public:
	void UpdateOnline(float dt);
	void UpdateOffline(float dt);

public:
	bool HandleEvent(const InputState& state);

private:
	EventBus& m_mModelEvents;
	GameState& m_mState;
	ClientNetworkSystem&	m_mNetwork;

private:
	std::unique_ptr<AIManager> m_mAIManager;
	std::vector<std::unique_ptr<Command>> m_mCommand;
};