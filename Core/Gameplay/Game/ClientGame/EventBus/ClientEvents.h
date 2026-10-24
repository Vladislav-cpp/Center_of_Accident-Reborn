#pragma once

struct ConnectionStateChangedEvent {

};

struct ReturnToInterfaceRequestedEvent {

};

struct OfflineGameRequestedEvent {

};

struct OnlineGameRequestedEvent {

};

// data for UI; ONLY const value

class PlayerCharacter;

struct OfflineGameStart {
	const std::shared_ptr<PlayerCharacter> player;
};