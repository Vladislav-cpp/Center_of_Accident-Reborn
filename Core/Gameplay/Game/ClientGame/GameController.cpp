#include "GameController.h"

#include <SFML/Window/Event.hpp>

#include "ClientNetworkSystem.h"
#include "GameState.h"

#include "CharacterFactory.h"
#include "StaticStatsUfo.h"
#include "CollisionSystem.h"
#include "HealthSystem.h"
#include "HumenPlayerController.h"
#include "ActionHandler.h"
#include "World.h"

void GameController::InitializeOnline() {

	// create local player


}

void GameController::InitializeOffline() {
	// create local player
	m_mState.m_xPlayer = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>();
	
	m_mState.m_xPlayer->SetCoord({100, 100} );

	m_mAIManager = std::make_unique<AIManager>();

	//TODO додати скрипт як спавнити аі і таке всяке, поки просто заспавним вручну
	SpawnPoint point1;

	point1.m_iBotsPerSpawn	= 3;
	point1.m_iMaxBots		= 20;
	point1.m_vPos =			{500, 500};

	m_mAIManager->AddSpawnPoint(point1);

}

void GameController::UpdateOnline(float dt) {

}

void GameController::UpdateOffline(float dt) {

	//for( auto& ai : WAIPlayers() ) for( auto& comand : ai->GetController()->GenerateCommands(dt) ) comand->Execute();

	for(auto& c : m_mCommand) {

		if(MovementCommand* movement = dynamic_cast<MovementCommand*>(c.get()) ) {
			movement->SetDT(dt);
		}

		c->Execute();
	}

	m_mAIManager->Update(dt);

	for( auto& ai : WProjectiles() ) for( auto& comand : ai->GetController()->GenerateCommands(dt) ) comand->Execute();

	CollSys().UpdateCollisions();
	HealthSys().Update();
}

bool GameController::HandleEvent(const sf::Event& event) {


	ActionHandler::Instance().ActionsImput(event);

	InputState input = ActionHandler::Instance().Update();
	
	auto commands = m_mState.m_xPlayer->GetHumenController().GenerateCommands(input);

	if( commands.empty() ) return false;

	m_mCommand = std::move(commands);
	return true;
}
