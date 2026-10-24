#include "HumenPlayerController.h"
#include "InputState.h"
#include <SFML/Graphics.hpp>
#include "GameTime.h"
#include "StaticObject.h"

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


std::vector<std::unique_ptr<Command>> HumenPlayerController::GenerateCommands(const InputState& state) {
	std::vector<std::unique_ptr<Command>> commands;

	if( auto moveCommand = CreateMoveCommand(state); moveCommand != nullptr )		commands.push_back( std::move(moveCommand) );
	if( auto attackCommand = CreateAttackCommand(state); attackCommand != nullptr ) commands.push_back( std::move(attackCommand) );

	return commands;
}

std::unique_ptr<MovementCommand> HumenPlayerController::CreateMoveCommand(const InputState& state) {
	if( !state.bMove ) return nullptr;

	auto comand = std::make_unique<MovementCommand>();
	
	auto [offsetX, ofsetY] = state.moveOffset;

	comand->SetShift( {offsetX, ofsetY} );
	comand->SetObject( m_pObject );

	return comand;
}

std::unique_ptr<FireCommand> HumenPlayerController::CreateAttackCommand(const InputState& state) {
	if( !m_pAttacktimer->IsFinished() ) return nullptr;

	if( !state.bFire ) return nullptr;

	m_pAttacktimer->Restart();
	auto comand = std::make_unique<FireCommand>();
	
	comand->SetMousePosition( state.mousePosition );
	comand->SetObject( m_pObject );

	return comand;
}