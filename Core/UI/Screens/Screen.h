#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "ScreenManager.h"

class Screen {
	public:
	Screen(sf::RenderWindow& window, ScreenManager& screens)
		: m_xWindow(window), m_xScreens(screens) {}
	virtual ~Screen() = default;

	public:
	virtual void OnOpen() {}
	virtual void OnClose() {}

	public:
	virtual void HandleEvent(const sf::Event&) = 0;
	virtual void Render(float dt) = 0;

	protected:
	virtual void Update(float dt) = 0;

	protected:
	sf::RenderWindow& m_xWindow;
	ScreenManager& m_xScreens;
};
