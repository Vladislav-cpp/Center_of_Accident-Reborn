#pragma once
#include <memory>
#include <vector>

class Screen;
class GameSession;

class ScreenManager {
	public:
	ScreenManager(GameSession& session) : m_xSession(session) {}

	public:
	void Push(std::unique_ptr<Screen>);
	void Pop();

	public:
	Screen* Current();

	public:
	GameSession& m_xSession;

	private:
	std::vector<std::unique_ptr<Screen>> m_xStack;
};