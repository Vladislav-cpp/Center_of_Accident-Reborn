#pragma once
#include <memory>

#include <SFML/Graphics.hpp>
class PlayerCharacter;

enum class GameState {
    MainMenu,
    InLobby,   // з'Їднан≥, World ще не наповнений/не синхрон≥зований
    InGame     // персонаж заспавнений ≥ World синхрон≥зований Ч граЇмо
};

class GameSession {
	public:
	std::shared_ptr<PlayerCharacter> m_xPlayer;
	GameState state = GameState::MainMenu;
	
	sf::View view;
};


