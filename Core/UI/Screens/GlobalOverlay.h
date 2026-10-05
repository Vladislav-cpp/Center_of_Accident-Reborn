#pragma once
#include "UIElement.h"
#include "Screen.h"
#include "net_connectionState.h"


class GlobalOverlay : public Screen {
	public:
	GlobalOverlay(sf::RenderWindow& window, ScreenManager& screens);

	virtual bool HandleEvent(const sf::Event& e) override { return true; }; // eate all event !!!!
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;

	void RefreshStatus(net::ConnectionState state);

	private:
	UIElement* m_uConnectionStatus = nullptr;

};
