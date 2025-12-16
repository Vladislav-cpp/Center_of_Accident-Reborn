#pragma once
#include "Screen.h"
#include "UIElement.h"

class MapHUD;

class GameScreen : public Screen {
	public:
	GameScreen(sf::RenderWindow& window, ScreenManager& screens);
	~GameScreen();

	virtual void HandleEvent(const sf::Event& e) override;
	virtual void Update(float dt) override;
	virtual void Render(float dt) override;

	private:
	UIElement m_uBackground;

	// заготовка під HUD
	MapHUD* m_pHUD;
};
