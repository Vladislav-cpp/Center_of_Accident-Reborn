#include "ServerGame.h"
#include "ServerNetworkSystem.h"
#include "World.h"
#include "DestructibleObject.h"
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

void ServerGame::Run() {
    m_xNetwork->Start();

    m_xThreads.AddTask(kTickHz, [this](float dt) { Tick(dt); });
    m_xThreads.Start();

    // Nothing to do on this thread: wait until Stop() is called
    while( m_xThreads.IsRunning() ) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// ---------------------------------------------------------------- the single task

void ServerGame::Tick(float dt) {

    m_xNetwork->Update(-1);                         // 1. receive packet // 2. process packets, connects, disconnects
    UpdateMatchState(dt);                           // 3. state transitions (uses the fresh player count)

    if(m_eState == MatchState::InProgress) {

        SimulateWorld(dt);                          // 4. simulation
        m_xNetwork->OnSyncWorldState();             // 5. sync world state
        m_xNetwork->BroadcastWorld();               // 6. send to clients
    }
}

// ---------------------------------------------------------------- match logic

void ServerGame::SimulateWorld(float dt) {
    for( auto& ai : WAIPlayers() ) {
        for(auto& c : ai->GetController()->GenerateCommands(dt)) c->Execute();
    }

    for( auto& pr : WProjectiles() ) {
        for (auto& c : pr->GetController()->GenerateCommands(dt)) c->Execute();
    }

    CollSys().UpdateCollisions();
    HealthSys().Update(m_xNetwork);
}

void ServerGame::UpdateMatchState(float dt) {
    const uint32_t players = static_cast<uint32_t>(WHumanPlayers().size());

    switch (m_eState) {

    case MatchState::WaitingForPlayers:
        if (players >= m_xConfig.minPlayers) {
            SetState(MatchState::Countdown);
        }

        break;

    case MatchState::Countdown:

        if (players < m_xConfig.minPlayers) {
            SetState(MatchState::WaitingForPlayers);
            break;
        }

        StartMatch();
        SetState(MatchState::InProgress);
        
        break;

    case MatchState::InProgress:
        if( CheckWinCondition() ) {
            SetState(MatchState::Finished);
        }

        break;

    case MatchState::Finished:

        ResetMatch();
        SetState(MatchState::WaitingForPlayers);

        break;
    }
}

void ServerGame::SetState(MatchState s) {   
    if( s == m_eState) return;

    m_eState = s;

    net::message<MsgTypes> msg;
	msg.header.id = MsgTypes::S2C_MatchState;
    msg << m_eState;

    m_xNetwork->MessageAllClients(msg);
}

void ServerGame::StartMatch() {
    SpawnPoint p;
    p.m_vPos          = { 600, 600 };
    p.m_iMaxBots      = 500;
    p.m_iBotsPerSpawn = 5;
    p.m_xSpawnTimer   = GameTimer{ 3 };
    // m_xAIManager->AddSpawnPoint(p);


    m_xNetwork->OnStartMatch();

}

void ServerGame::ResetMatch() {
    // TODO: clear projectiles and AI, reset scores, reinitialize players
}

bool ServerGame::CheckWinCondition() {
    // TODO: depends on your game mode
    return false;
}