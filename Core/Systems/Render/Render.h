#pragma once
#include <SFML/Graphics.hpp>

class Render {
	public:
	Render(sf::RenderWindow& window) : window(window) {}

	public:
	void DrawWorld(float time, bool drawDebug = false);

	private:
	sf::RenderWindow& window;
};