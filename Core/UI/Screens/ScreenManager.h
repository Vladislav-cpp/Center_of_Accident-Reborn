#pragma once
#include <memory>
#include <vector>

class Screen;
class EventBus;

namespace sf {
	class Event;
}	


class ScreenManager {
	public:
	ScreenManager( EventBus& events ) : m_mEvents(events) {}

	public:
	void Initialize();

	public:
	void Draw(float dt);
	bool HandleEvent(const sf::Event& e);

	private:
	void Push(std::unique_ptr<Screen>);
	void Pop();

	private:
	Screen* Current();

	private:
	EventBus& m_mEvents;

	private:
	std::vector<std::unique_ptr<Screen>> m_xStack;
};