#include "GameScreen.h"
#include "MapHUD.h"
#include "Config.h"
#include "PlayerCharacter.h"
#include "Utility.h"
#include "SoundManager.h"

static const std::string m_sMusic = "Resources/sound and music/background music.ogg";


GameScreen::GameScreen(sf::RenderWindow& window, std::shared_ptr<EventBus> m_mUIEvents) : Screen(window, m_mUIEvents) {

	m_xSubType = ScreenSubType::GameScreen;

	m_uBackground.SetData("Resources/images/background.png", {0, 0},{ CFG().mapWidth , CFG().mapHeight }, 0, 0 );


	// --- MUSIC ---
	//SoundMg().PlayMusic( m_sMusic, true, 70.f );

	// TODO m_pHUD = new MapHUD(m_xScreens.m_xSession.m_xPlayer);

	m_mUIEvents->Subscribe<HUDInitData>([this](const HUDInitData& uiData) {
		m_pHUD = new MapHUD(uiData.player);
		this->player = uiData.player;
	});

}

GameScreen::~GameScreen() {
	delete m_pHUD;
}

bool GameScreen::HandleEvent(const sf::Event& e) {
	// натискання клавіш, пауза, відкриття меню і т.д.
	// приклад: якщо Escape → m_Screens.Push(std::make_unique<PauseScreen>(m_Window, m_Screens));
	return false;
}

void GameScreen::Update(float dt) {
	if(!m_mView) return;
	m_mView->setCenter( utility::ClampToMap( player->Coord(), CFG().windowWidth, CFG().windowHeight, CFG().mapWidth, CFG().mapHeight) );

	m_xWindow.setView(*m_mView);
}

void GameScreen::Draw(float dt) {
	Update(dt);
	
	m_uBackground.Draw(m_xWindow);

	if( m_pHUD ) m_pHUD->Draw(m_xWindow, dt);
}
