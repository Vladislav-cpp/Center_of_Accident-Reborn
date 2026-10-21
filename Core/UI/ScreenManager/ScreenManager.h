#pragma once
#include <memory>
#include <vector>
#include <Screen.h>
#include <UIEventEvents.h>
#include <EventBus.h>
#include <SFML/Graphics.hpp>

#include "GameScreen.h"
#include "MainMenuScreen.h"
#include "GlobalOverlay.h"
#include "Config.h"
#include "ActionHandler.h"
#include "ClientEvents.h"

namespace sf {
	class Event;
}	


class ScreenManager {
public:
	ScreenManager(sf::RenderWindow* window,  EventBus& events ) : m_mWindow(window), m_mModelEvents(events) {}

public:
	void Draw(float dt);
	bool HandleEvent(const sf::Event& e);

private:
	void Push(std::unique_ptr<Screen>);
	void Pop();

private:
	Screen* Current();

private:
	EventBus& m_mModelEvents;
	std::shared_ptr<EventBus> m_mUIEvents = nullptr;

private:
	std::vector<std::unique_ptr<Screen>> m_xStack;

	sf::View m_mView;

	private:

public:
	void Initialize();

private:
	void OnButtonPressed(const ButtonPressed& event);
	void SubscribeToEvents();

private:
	GameScreen*			m_mGameScreen		= nullptr;
	GlobalOverlay*		m_mGlobalOverlay	= nullptr;
	MainMenuScreen*		m_mMainMenuScreen	= nullptr;

	sf::RenderWindow*	m_mWindow			= nullptr;
};




