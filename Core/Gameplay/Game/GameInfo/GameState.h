#pragma once
#include <memory>

class PlayerCharacter;

enum class GamePhase {
    MainMenu,
    InLobby,   // з'Їднан≥, World ще не наповнений/не синхрон≥зований
    InGame     // персонаж заспавнений ≥ World синхрон≥зований Ч граЇмо
};

enum class ApplicationMode {
	Interface,
	OnlineGame,
	OfflineGame
};

enum class PlayerConnState { 
	Connecting,
	Disconnected
};

enum class MatchState {
    WaitingForPlayers,
    Countdown,
    InProgress,
    Finished
};

struct NetData {
    uint32_t id = 0;
    PlayerConnState state = PlayerConnState::Disconnected;
	MatchState mstate = MatchState::WaitingForPlayers;
};

class GameState {
	public:
	std::shared_ptr<PlayerCharacter> m_xPlayer;
	std::shared_ptr<NetData> m_xNetData;

	public:
	GamePhase phase = GamePhase::MainMenu;
	ApplicationMode mode = ApplicationMode::Interface;
};


