#pragma once

#include <SFML/System/Vector2.hpp>

struct InputState {
	bool bMove = false;
	sf::Vector2f moveOffset {0, 0};

	bool bFire = false;
	sf::Vector2f mousePosition {0, 0};
};

