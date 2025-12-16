#pragma once

#include "Command.h"
#include <SFML/System/Vector2.hpp>


class MovementCommand : public Command {
	public:
	virtual void Execute() override;
	virtual void SetShift(sf::Vector2f ShiftXY);

	private:
	sf::Vector2f m_fShiftXY;
};

