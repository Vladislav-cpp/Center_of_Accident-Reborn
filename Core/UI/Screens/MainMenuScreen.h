#pragma once
#include "Screen.h"
#include "UIElement.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class EventBus;

class MainMenuScreen : public Screen {
	public:
	MainMenuScreen(sf::RenderWindow& window, std::shared_ptr<EventBus> m_mUIEvents);

	virtual bool HandleEvent(const sf::Event& e) override;

	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;

	// TODO зробити лямбду в класі UIElement на нажаття
	void OnStartOffline();
	void OnStartOnline();
	void OnOption();

	virtual void OnOpen() override {};
	virtual void OnClose() override {};

	private:
	UIElement m_uMenu; // TODO need be pointer
	UIElement m_uStartOffline;
	UIElement m_uStartOnline;
	UIElement m_uOption;

	private:
	sf::Cursor m_xCursor;

	private:
	sf::View m_xView;

	private:
	bool m_bOverStart = false;
	bool m_bOverOption = false;
};
