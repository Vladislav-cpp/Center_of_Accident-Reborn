#include "ServerGame.h"
#include "ServerNetworkSystem.h"
#include "World.h"
#include "DestructibleObject.h"
#include "HumenPlayerController.h"
#include "AIPlayerController.h"
#include "GameTime.h"
#include "CollisionSystem.h"
#include "SpawnSystem.h"
#include "HealthSystem.h"

ServerGame::ServerGame() {	
	m_xNetwork = new ServerNetworkSystem(60000);
	m_xSpawner = new SpawnSystem();
}

ServerGame::~ServerGame() {
	delete m_xNetwork;
	delete m_xSpawner;
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
	GameTime globalTime;

	SpawnPoint p;
	p.m_vPos = {600 , 600};
	p.m_iMaxBots = 500;
	p.m_iBotsPerSpawn = 5;
	p.m_xSpawnTimer = GameTimer{3};	

	m_xSpawner->AddSpawnPoint(p);

	GameTimer tickTimer(0.0166); // Ëîã³êà òà Ò³ê (60 Ãö)
	GameTimer netRate(0.033); // 30 Ãö

	while(true) {

		globalTime.Update();
		float dt = globalTime.Delta();

		m_xNetwork->Update(-1);

		if( tickTimer.IsFinished() ) {

			m_xNetwork->OnTick();

			if( !WHumanPlayers().empty() ) m_xSpawner->Update();

			for( auto& ai : WAIPlayers() ) for( auto& comand : ai->GetController()->GenerateCommands(dt) ) comand->Execute();
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