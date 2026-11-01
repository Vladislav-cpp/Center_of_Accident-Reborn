#pragma once

#include "PeriodicThreads.h"
#include <chrono>
#include <cstdint>
#include <thread>
#include <GameState.h>

class ServerNetworkSystem;
class AIManager;



struct MatchConfig {
    uint32_t minPlayers   = 2;
    uint32_t maxPlayers   = 16;
};

class ServerGame {
	public:
	ServerGame();
	~ServerGame();

public:
    void Run();
    void Stop() { m_xThreads.RequestStop(); }

private:
    void Tick(float dt);          // the single server step

    void UpdateMatchState(float dt);
    void SimulateWorld(float dt);
    void SetState(MatchState s);
    void StartMatch();
    void ResetMatch();
    bool CheckWinCondition();

private:

    static constexpr double kTickHz = 120.0;   // the one rate for everything

    MatchConfig m_xConfig;
    MatchState  m_eState      = MatchState::WaitingForPlayers;

    PeriodicThreads m_xThreads;   // declared last: joined first on destruction

private:
	ServerNetworkSystem* m_xNetwork;
	AIManager* m_xAIManager;
};