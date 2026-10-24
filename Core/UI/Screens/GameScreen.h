#pragma once
#include "Screen.h"
#include "UIElement.h"

class MapHUD;
class EventBus;
class PlayerCharacter;

class GameScreen : public Screen {
	public:
	GameScreen(sf::RenderWindow& window, std::shared_ptr<EventBus> m_mUIEvents);
	~GameScreen();

	virtual bool HandleEvent(const sf::Event& e) override;
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;

	virtual void OnOpen() override {};
	virtual void OnClose() override {};

	void SetView(sf::View* view) {m_mView = view; };

	private:
	UIElement m_uBackground;

	// заготовка під HUD
	MapHUD* m_pHUD = nullptr;
	sf::View* m_mView = nullptr;
	std::shared_ptr<const PlayerCharacter> player = nullptr;
};
