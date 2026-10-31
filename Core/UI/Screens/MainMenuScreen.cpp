#include "MainMenuScreen.h"
#include "Config.h"
#include "SoundManager.h"

static const std::string m_sMusicMenu = "Resources/sound and music/music for the menu.ogg";

MainMenuScreen::MainMenuScreen(sf::RenderWindow& window, std::shared_ptr<EventBus> m_mUIEvents) 
							: Screen(window, m_mUIEvents), m_xCursor(sf::Cursor::Type::Arrow) {

	m_xSubType = ScreenSubType::MainMenu;

	// --- BACKGROUND ---
	m_uMenu.SetData("Resources/images/menu/game__menu.png", {0, 0}, { CFG().mapWidth, CFG().mapHeight }, 0, 0 );// прибрати це має бути в MainMenuScreen
	m_uMenu.GetxTexture().setSmooth(true);

	// --- BUTTON ---
	m_uStartOffline.SetData("Resources/images/menu/start.png", {0, 0}, {227, 152}, 0, 0 ); // по факту це овери, а кнопка зашита в самий m_uMenu
	m_uStartOffline.GetxTexture().setSmooth(true);
	m_uStartOffline.SetCoord({ 427.f, 130.f });
	m_uStartOffline.m_xType = UIType::MainMenuStartOffline;

	m_uOption.SetData("Resources/images/menu/option.png", {0, 0}, {227, 152}, 0, 0 );
	m_uOption.GetxTexture().setSmooth(true);
	m_uOption.SetCoord({ 427.f, 130.f });
	m_uOption.m_xType = UIType::MainMenuOption;


	// --- MOCK START ONLINE ---
	m_uStartOnline.CreateSimpleSquare({ 130u, 50u }, sf::Color::Green);
	m_uStartOnline.SetCoord({ 460.f, 304.f });
	m_uStartOnline.m_xType = UIType::MainMenuStartOnline;

	// --- MUSIC ---
	//SoundMg().PlayMusic( m_sMusicMenu, true, 70.f );

	// --- CURSOR ---
	m_xWindow.setMouseCursor(m_xCursor);

	// --- VIEW ---
	auto size = m_xWindow.getSize();

	m_xView = sf::View(
		sf::Vector2f{ size.x * 0.5f, size.y * 0.5f },
		sf::Vector2f{ float(size.x), float(size.y) }
	);

	m_xView.setViewport( { {0.f, 0.f}, {1.f, 1.f} } );
	m_xWindow.setView(m_xView);
}

bool MainMenuScreen::HandleEvent(const sf::Event& e) {
	const auto* mouse = e.getIf<sf::Event::MouseButtonPressed>();
	if( mouse == nullptr || mouse->button != sf::Mouse::Button::Left ) return false;
	
	auto mousePos = sf::Mouse::getPosition(m_xWindow);
/*
	if( m_uStartOffline.Contains(mousePos) ) {
		OnStart();
		return true;

	} else if( m_uOption.Contains(mousePos) )  {
		OnOption();
		return true;
	}
*/

	//return false;
	if( sf::IntRect({435, 146}, {191, 49}).contains(mousePos) ) {
		OnStartOffline();
		return true;

	} else if( sf::IntRect({451, 216}, {159, 49}).contains(mousePos) )  {
		OnOption();
		return true;

	} else if( m_uStartOnline.Contains(mousePos) )  {
		OnStartOnline();
		return true;
	}

	return false;
}

void MainMenuScreen::Update(float) {
	auto mousePos = sf::Mouse::getPosition(m_xWindow);

	m_bOverStart = sf::IntRect(  {435, 146}, {191, 49} ).contains(mousePos);
	m_bOverOption = sf::IntRect( {451, 216}, {159, 49} ).contains(mousePos);
}

void MainMenuScreen::Draw(float dt) {
	Update(dt);

	m_uMenu.Draw(m_xWindow);

	if( m_bOverStart ) m_uStartOffline.Draw(m_xWindow);
	if( m_bOverOption ) m_uOption.Draw(m_xWindow);

	m_uStartOnline.Draw(m_xWindow);
}

void MainMenuScreen::OnStartOffline() {
	
	m_mUIEvents->Publish( ButtonPressed{ m_uStartOffline.m_xType } );

	//SoundMg().StopMusic( m_sMusicMenu );

	//ClientGame::SendGameRequest()
}

void MainMenuScreen::OnStartOnline() {
	
	m_mUIEvents->Publish( ButtonPressed{ m_uStartOffline.m_xType } );

	//SoundMg().StopMusic( m_sMusicMenu );

	//ClientGame::SendGameRequest()
}

void MainMenuScreen::OnOption() {
	
	m_mUIEvents->Publish( ButtonPressed{ m_uOption.m_xType } );

	//SoundMg().StopMusic( m_sMusicMenu );
}