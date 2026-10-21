#pragma once
#include "UIElement.h"
#include "Screen.h"
#include "net_connectionState.h"

class EventBus;

class GlobalOverlay : public Screen {
	public:
	GlobalOverlay(sf::RenderWindow& window, std::shared_ptr<EventBus> m_mUIEvents);

	virtual bool HandleEvent(const sf::Event& e) override { return true; }; // eate all event !!!!
	virtual void Update(float dt) override;
	virtual void Draw(float dt) override;

	void RefreshStatus(net::ConnectionState state);

	virtual void OnOpen() override {};
	virtual void OnClose() override {};

	private:
	UIElement* m_uConnectionStatus = nullptr;

};
