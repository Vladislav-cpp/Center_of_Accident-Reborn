#include "AIPlayerController.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
#include "ActionHandler.h"
#include "PlayerCharacter.h"
#include "AICharacter.h"


std::vector<std::unique_ptr<Command>> AIPlayerController::GenerateCommands(float dt) {
	std::vector<std::unique_ptr<Command>> commands;

	for( auto& behavior : m_vBehaviors ) {
		behavior->ai = std::dynamic_pointer_cast<AICharacter>(m_pObject);
		behavior->pl = player;

		for( auto& comand : behavior->GenerateCommands(dt) ) commands.push_back( std::move(comand) );
	}
 
	return commands;
}

void AIPlayerController::SetEnemyPlayer(const std::shared_ptr<PlayerCharacter>& pl) {
	player = pl;
}

void AIPlayerController::AddBehavior(std::vector<std::shared_ptr<IBehaviorState>>& behaviors) {
	for(auto& behavior : behaviors) m_vBehaviors.push_back( behavior );
}

	//if( auto moveCommand = CreateMoveCommand(); moveCommand != nullptr ) commands.push_back( std::move(moveCommand) );
//std::unique_ptr<MovementCommand> AIPlayerController::CreateMoveCommand() {
//	auto& game = Game::Instance();
//
//	float time = game.GetTime();
//	time /= m_pObject->GetPrototpe()->speedMovement.value();
//
//	//відстань до обєкта якого ми рухаємось
//	auto [targetX, targetY] = game.GetPlayer()->Coord();
//	auto [oldMyX, oldMyY] = m_pObject->Coord();
//	float distance = sqrt( pow(targetX - oldMyX, 2) + pow(targetY - oldMyY, 2) );
//
//	if(distance<5) return nullptr; //задля не забагуавння на місті огранічцєм
//	
//	auto newMyX = oldMyX + (time * (targetX - oldMyX ) / distance);
//	auto newMyY = oldMyY + (time * (targetY - oldMyY ) / distance);
//
//	auto comand = std::make_unique<MovementCommand>();
//	comand->SetObject(m_pObject);
//	comand->SetShift( {newMyX - oldMyX, newMyY - oldMyY} );
//	
//	return comand;
//} 