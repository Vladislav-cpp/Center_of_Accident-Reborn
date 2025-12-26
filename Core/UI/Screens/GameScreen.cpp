#include "GameScreen.h"
#include "GameSession.h"
#include "MapHUD.h"
#include "Config.h"
#include "PlayerCharacter.h"
#include "Utility.h"
#include "SoundManager.h"

static const std::string m_sMusic = "Resources/sound and music/background music.ogg";


GameScreen::GameScreen(sf::RenderWindow& window, ScreenManager& screens) : Screen(window, screens) {

	m_uBackground.SetData("Resources/images/background.png", {0, 0},{ CFG().mapWidth , CFG().mapHeight }, 0, 0 );


	// --- MUSIC ---
	//SoundMg().PlayMusic( m_sMusic, true, 70.f );

	m_pHUD = new MapHUD(m_xScreens.m_xSession.m_xPlayer);
}

GameScreen::~GameScreen() {
	delete m_pHUD;
}

void GameScreen::HandleEvent(const sf::Event& e) {
	// натискання клавіш, пауза, відкриття меню і т.д.
	// приклад: якщо Escape → m_Screens.Push(std::make_unique<PauseScreen>(m_Window, m_Screens));
}

void GameScreen::Update(float dt) {
	m_xScreens.m_xSession.view.setCenter( utility::ClampToMap( m_xScreens.m_xSession.m_xPlayer->Coord(), CFG().windowWidth, CFG().windowHeight, CFG().mapWidth, CFG().mapHeight) );
}

void GameScreen::Render(float dt) {
	Update(dt);
	
	m_uBackground.Draw(m_xWindow);

	if( m_pHUD ) m_pHUD->Draw(m_xWindow, dt);
}
