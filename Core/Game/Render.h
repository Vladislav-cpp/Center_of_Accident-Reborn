#pragma once
#include <SFML/Graphics.hpp>

class GameSession;

class Render {
	public:
	Render(sf::RenderWindow& window, GameSession& session) : window(window), m_xSession(session) {}

	public:
	void DrawWorld(float time, bool drawDebug = false);

	private:
	sf::RenderWindow& window;
	GameSession& m_xSession;
};