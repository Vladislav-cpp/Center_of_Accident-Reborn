#include "HumenPlayerController.h"
#include "InputState.h"
#include <SFML/Graphics.hpp>
#include "ActionHandler.h"
#include "GameTime.h"
#include "StaticObject.h"

#define Key sf::Keyboard::Key // sf::Mouse::Button::Left
#define isKeyPressed sf::Keyboard::isKeyPressed
#define Button sf::Mouse::Button
#define isButtonPressed sf::Mouse::isButtonPressed


HumenPlayerController::HumenPlayerController() {
	m_pAttacktimer = new GameTimer();
}

HumenPlayerController::~HumenPlayerController() {
	delete m_pAttacktimer;
}

void HumenPlayerController::SetPlayer(std::shared_ptr<StaticObject> Object) {
	IPlayerController::SetPlayer(Object);

	m_pAttacktimer->Restart( m_pObject->GetPrototpe()->rechargeTime.value());
}


std::vector<std::unique_ptr<Command>> HumenPlayerController::GenerateCommands(float dt) {
	std::vector<std::unique_ptr<Command>> commands;

	if( auto moveCommand = CreateMoveCommand(dt); moveCommand != nullptr )		commands.push_back( std::move(moveCommand) );
	if( auto attackCommand = CreateAttackCommand(); attackCommand != nullptr ) commands.push_back( std::move(attackCommand) );

	return commands;
}

std::unique_ptr<MovementCommand> HumenPlayerController::CreateMoveCommand(float dt) {
	auto state = ActionHandler::Instance().ActionsImput();
	if( !state.bMove ) return nullptr;

	auto comand = std::make_unique<MovementCommand>();
	
	float shift = dt * m_pObject->GetPrototpe()->speedMovement.value();
	auto [offsetX, ofsetY] = state.moveOffset;

	comand->SetShift( {offsetX * shift, ofsetY * shift} );
	comand->SetObject( m_pObject );

	return comand;
}

std::unique_ptr<FireCommand> HumenPlayerController::CreateAttackCommand() {
	if( !m_pAttacktimer->IsFinished() ) return nullptr;
	m_pAttacktimer->Restart();

	auto state = ActionHandler::Instance().ActionsImput();
	if( !state.bFire ) return nullptr;
	auto comand = std::make_unique<FireCommand>();
	
	comand->SetMousePosition( state.mousePosition );
	comand->SetObject( m_pObject );

	return comand;
}