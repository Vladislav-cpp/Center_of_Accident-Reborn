#include "ActionHandler.h"
#include "MovementCommand.h"
#include "FireCommand.h"
#include "Game.h"
#include "InputState.h"
#include <SFML/Window.hpp>

void ActionHandler::SetWindow(sf::RenderWindow* window) {
	m_xWindow = window;
}

#define Key sf::Keyboard::Key // sf::Mouse::Button::Left
#define isKeyPressed sf::Keyboard::isKeyPressed
#define Button sf::Mouse::Button
#define isButtonPressed sf::Mouse::isButtonPressed

std::tuple<bool, sf::Vector2f> ActionHandler::GetMovementInput() {
	sf::Vector2f moveDir(0.f, 0.f);

	if( isKeyPressed(Key::W) ) moveDir.y -= 1.f; //|| isKeyPressed(Key::Up	)
	if( isKeyPressed(Key::S) ) moveDir.y += 1.f; //|| isKeyPressed(Key::Down	)
	if( isKeyPressed(Key::A) ) moveDir.x -= 1.f; //|| isKeyPressed(Key::Left	)
	if( isKeyPressed(Key::D) ) moveDir.x += 1.f; //|| isKeyPressed(Key::Right	)

	bool isMoving = (moveDir.x != 0.f || moveDir.y != 0.f);
	return { isMoving, moveDir };
}

std::tuple<bool, sf::Vector2f> ActionHandler::GetFireInput() {
	return { isButtonPressed(Button::Left), m_xWindow->mapPixelToCoords(sf::Mouse::getPosition( *m_xWindow )) };
}

InputState ActionHandler::ActionsImput() {
	if(m_bFilledState) return m_xState;
	else m_bFilledState = true;

	std::tie( m_xState.bMove, m_xState.moveOffset )		= GetMovementInput();
	std::tie( m_xState.bFire, m_xState.mousePosition )	= GetFireInput();

 	return m_xState;
}

void ActionHandler::Clean() {
	m_bFilledState = false;

	m_xState.bFire = false;
	m_xState.bMove = false;
	m_xState.mousePosition = {0, 0};
	m_xState.moveOffset = {0, 0};
}
