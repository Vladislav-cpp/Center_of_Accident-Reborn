#pragma once
#include "Command.h"
#include <SFML/System/Vector2.hpp>

class FireCommand : public Command {
	public:
	virtual void Execute() override;
	virtual void SetMousePosition(sf::Vector2f position);
	virtual sf::Vector2f GetMousePosition();

	protected:
	sf::Vector2f m_vMousePosition {0, 0};
};

