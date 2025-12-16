#pragma once
#include <memory>

#include <SFML/Graphics.hpp>
class PlayerCharacter;

class GameSession {
	public:
	std::shared_ptr<PlayerCharacter> m_xPlayer;

	sf::View view;
};
