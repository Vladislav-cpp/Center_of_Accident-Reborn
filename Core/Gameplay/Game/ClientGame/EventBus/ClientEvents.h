#pragma once

#include <GameState.h>

struct JoinAcceptedEvent {
	// S2C_JoinAccepted
};

struct ReturnToInterfaceRequestedEvent {

};

struct OfflineGameRequestedEvent {

};

struct OnlineGameRequestedEvent {

};

// data for UI; ONLY const value

class PlayerCharacter;

struct OfflineGameStart {// TODO поміняти назву
	const std::shared_ptr<PlayerCharacter> player;
};

struct OnlineGameStart {
	const std::shared_ptr<PlayerCharacter> player;
};

// online S2C messege
struct MatchStateInf {
	MatchState state = MatchState::WaitingForPlayers;
	uint32_t m_iMyID = 0;
};
