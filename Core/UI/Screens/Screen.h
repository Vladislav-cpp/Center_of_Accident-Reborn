#pragma once
#include <SFML/Window/Event.hpp>
#include <UIElement.h>
#include <UIEventEvents.h>
#include <EventBus.h>

class ScreenManager;

class Screen : public UIElement  {
	public:
	Screen(sf::RenderWindow& window, std::shared_ptr<EventBus> m_mUIEvents) : m_xWindow(window), m_mUIEvents(m_mUIEvents) {
		m_xType = UIType::Screen;
	}

	virtual ~Screen() = default;

	public:
	virtual void OnOpen() = 0;
	virtual void OnClose() = 0;

	public:
	virtual bool HandleEvent(const sf::Event&) = 0;
	virtual void Draw(float dt = 0) = 0;

	protected:
	virtual void Update(float dt = 0) = 0;

	protected:
	sf::RenderWindow& m_xWindow;
	std::shared_ptr<EventBus> m_mUIEvents = nullptr;
};
