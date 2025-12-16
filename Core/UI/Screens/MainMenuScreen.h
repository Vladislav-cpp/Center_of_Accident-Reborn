#pragma once
#include "Screen.h"
#include "UIElement.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>



class MainMenuScreen : public Screen {
	public:
	MainMenuScreen(sf::RenderWindow& window, ScreenManager& screens);

	virtual void HandleEvent(const sf::Event& e) override;
	virtual void Update(float dt) override;
	virtual void Render(float dt) override;

	bool IsStartGame() { return m_bStartGameRequested; };

	void OnStart();

	private:
	UIElement m_uMenu;
	UIElement m_uStar;
	UIElement m_uOption;

	private:
	sf::Cursor m_xCursor;

	private:
	sf::View m_xView;

	private:
	bool m_bHoverStart = false;
	bool m_bHoverOption = false;
	bool m_bStartGameRequested = false;
};
