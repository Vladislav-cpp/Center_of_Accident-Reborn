#pragma once
#include <memory>

#include <SFML/Graphics.hpp>
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
	Loading,
	InMatch,
	Disconnected
};

struct NetData {
    uint32_t id = 0;
    PlayerConnState state = PlayerConnState::Connecting;
    bool bReady = false;
};

class GameState {
	public:
	std::shared_ptr<PlayerCharacter> m_xPlayer;
	std::shared_ptr<NetData> m_xNetData;

	public:
	GamePhase state = GamePhase::MainMenu;
	ApplicationMode mode = ApplicationMode::Interface;
	
	sf::View view; // видалити цю д≥ч
};


