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

struct OfflineGameStart {// TODO поміняти назву
	const std::shared_ptr<PlayerCharacter> player;
};

struct OnlineGameStart {
	const std::shared_ptr<PlayerCharacter> player;
};