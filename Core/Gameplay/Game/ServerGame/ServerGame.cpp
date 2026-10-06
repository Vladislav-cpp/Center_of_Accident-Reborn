#include "ServerGame.h"
#include "ServerNetworkSystem.h"
#include "World.h"
#include "DestructibleObject.h"
#include "HumenPlayerController.h"
#include "AIPlayerController.h"
#include "TimeSystem.h"
#include "CollisionSystem.h"
#include "AIManager.h"
#include "HealthSystem.h"

ServerGame::ServerGame() {	
	m_xNetwork = new ServerNetworkSystem(60000);
	m_xAIManager = new AIManager();
}

ServerGame::~ServerGame() {
	delete m_xNetwork;
	delete m_xAIManager;
}


//struct SpawnPoint {
//	sf::Vector2f	m_vPos;
//
//	int m_iMaxBots = 0;
//	int m_iSpawned = 0;
//	int m_iBotsPerSpawn = 1;
//
//	GameTimer		m_xSpawnTimer;
//};
void ServerGame::Run() {
	m_xNetwork->Start();
	
	uint32_t UniqueBOT_ID = 0;

	SpawnPoint p;
	p.m_vPos = {600 , 600};
	p.m_iMaxBots = 500;
	p.m_iBotsPerSpawn = 5;
	p.m_xSpawnTimer = GameTimer{3};	

	m_xAIManager->AddSpawnPoint(p);

	GameTimer tickTimer(0.00833); // 60 Гц: 1 / 60 ≈ 0.0166, 120 Гц: 1 / 120 ≈ 0.00833, 144 Гц: 1 / 144 ≈ 0.00694, 240 Гц: 1 / 240 ≈ 0.00416
	GameTimer netRate(0.033); // 30 Гц

	while(true) {

		m_xNetwork->Update(-1);

		if( tickTimer.IsFinished() ) {
			tickTimer.Restart();

			TimeSys().Tick(tickTimer.Duration());
			float dt = TimeSys().DeltaTime();

			m_xNetwork->OnTick();

			//if( !WHumanPlayers().empty() ) m_xAIManager->Update();

			for( auto& ai : WAIPlayers() )   for( auto& comand : ai->GetController()->GenerateCommands(dt) ) comand->Execute();
			for( auto& ai : WProjectiles() ) for( auto& comand : ai->GetController()->GenerateCommands(dt) ) comand->Execute();

			CollSys().UpdateCollisions();
			HealthSys().Update(m_xNetwork);

			m_xNetwork->OnSyncWorldState();
		}

		if( netRate.IsFinished() ) {
			netRate.Restart();
			m_xNetwork->BroadcastWorld();
		}
	}

}