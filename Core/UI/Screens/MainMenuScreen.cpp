#include "MainMenuScreen.h"
#include "Config.h"
#include "SoundManager.h"

static const std::string m_sMusicMenu = "Resources/sound and music/music for the menu.ogg";

MainMenuScreen::MainMenuScreen(sf::RenderWindow& window, ScreenManager& screens) 
							: Screen(window, screens) , m_xCursor(sf::Cursor::Type::Arrow) {

	// --- BACKGROUND ---
	m_uMenu.SetData("Resources/images/menu/game__menu.png", {0, 0}, { CFG().mapWidth, CFG().mapHeight }, 0, 0 );
	m_uMenu.GetxTexture().setSmooth(true);

	// --- BUTTON ---
	m_uStar.SetData("Resources/images/menu/start.png", {0, 0}, {227, 152}, 0, 0 );
	m_uStar.GetxTexture().setSmooth(true);
	m_uStar.SetCoord({ 427.f, 130.f });

	m_uOption.SetData("Resources/images/menu/option.png", {0, 0}, {227, 152}, 0, 0 );
	m_uOption.GetxTexture().setSmooth(true);
	m_uOption.SetCoord({ 427.f, 130.f });


	// --- MUSIC ---
	SoundMg().PlayMusic( m_sMusicMenu, true, 70.f );

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

void MainMenuScreen::HandleEvent(const sf::Event& e) {
	const auto* mouse = e.getIf<sf::Event::MouseButtonPressed>();
	if( mouse == nullptr || mouse->button != sf::Mouse::Button::Left ) return;
	
	auto mousePos = sf::Mouse::getPosition(m_xWindow);

	if( sf::IntRect({435, 146}, {191, 49}).contains(mousePos) ) OnStart();
	else if( sf::IntRect({451, 216}, {159, 49}).contains(mousePos) )  /* OnOption() */;
}

void MainMenuScreen::Update(float) {
	auto mousePos = sf::Mouse::getPosition(m_xWindow);

	m_bHoverStart = sf::IntRect({435, 146}, {191, 49}).contains(mousePos);
	m_bHoverOption = sf::IntRect({451, 216}, {159, 49}).contains(mousePos);
}

void MainMenuScreen::Render(float dt) {
	Update(dt);

	m_uMenu.Draw(m_xWindow);

	if( m_bHoverStart ) m_uStar.Draw(m_xWindow);
	if( m_bHoverOption ) m_uOption.Draw(m_xWindow);
}

void MainMenuScreen::OnStart() {
	SoundMg().StopMusic( m_sMusicMenu );

	m_bStartGameRequested = true;
}