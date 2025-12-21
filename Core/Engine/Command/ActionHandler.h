#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <tuple>
#include "ImplSingleton.h"
#include "InputState.h"

class Command;

class ActionHandler : public ImplSingleton<ActionHandler> {
	friend class ImplSingleton<ActionHandler>;

	public:
	void SetWindow(sf::RenderWindow* window);

	public:
	InputState ActionsImput();
	void Clean();

	private:
	std::tuple<bool, sf::Vector2f> GetMovementInput();
	std::tuple<bool, sf::Vector2f> GetFireInput();

	private:
	bool m_bFilledState = false;
	InputState m_xState;
	sf::RenderWindow* m_xWindow;
};

