#pragma once
#include <memory>
#include <map>
#include <Screen.h>
#include <UIEventEvents.h>
#include <EventBus.h>
#include <SFML/Graphics.hpp>

#include "GameScreen.h"
#include "MainMenuScreen.h"
#include "GlobalOverlay.h"
#include "Config.h"
#include "ClientEvents.h"	


class ScreenManager {
public:
	ScreenManager(sf::RenderWindow* window,  EventBus& events ) : m_mWindow(window), m_mModelEvents(events) {}

public:
	void Draw(float dt);
	bool HandleEvent(const sf::Event& e);

private:
	void Push(std::unique_ptr<Screen>);

private:
	Screen* Current();
	std::vector<Screen*> OpenScreens();

private:
	EventBus& m_mModelEvents;
	std::shared_ptr<EventBus> m_mUIEvents = nullptr;

private:
	std::map<ScreenSubType, std::unique_ptr<Screen>> m_xStack;
	std::vector<ScreenSubType> m_xCurrentScreens;

	sf::View m_mView;

private: //TODO
public: // ...
	void Initialize();

private:
	void OnButtonPressed(const ButtonPressed& event);
	void SubscribeToEvents();

private:

	sf::RenderWindow*	m_mWindow			= nullptr;
};




