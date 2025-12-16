#pragma once
#include "UIElement.h"

class PlayerCharacter;

class MapHUD {
	public:
	MapHUD(std::shared_ptr<PlayerCharacter> pl);

	public:
	void Draw(sf::RenderWindow& app, int time = 0);

	private:
	void Update();

	private:
	UIElement* life;
	UIElement* coin;

	private:
	sf::Font font;
	sf::Text text—oin;
	sf::Text textLife;

	private:
	std::shared_ptr<PlayerCharacter> player;
};

