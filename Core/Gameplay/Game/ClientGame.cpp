#include "ClientGame.h"
#include "GameTime.h"
#include "Render.h"
#include "Config.h"
#include "ClientNetworkSystem.h"
#include "PlayerCharacter.h"
#include "FireCommand.h"
#include "MovementCommand.h"
#include "CharacterFactory.h"
#include "StaticStatsUfo.h"
#include "ActionHandler.h"
#include "World.h"
#include "IPlayerController.h"
#include "AICharacter.h"
#include "Projectile.h"
#include "ScreenManager.h"
#include "MainMenuScreen.h"
#include "GameSession.h"
#include "GameScreen.h"
#include "SoundManager.h"

ClientGame::ClientGame() {

	//auto fullscreenMode = sf::VideoMode::getFullscreenModes().front();
	//CFG().windowWidth = fullscreenMode.size.x;
	//CFG().windowHeight = fullscreenMode.size.y;

	CFG().windowWidth = 1000;
	CFG().windowHeight = 563;

	//m_xWindow = new sf::RenderWindow(fullscreenMode, "Accident Center 2", sf::State::Fullscreen ); // 1000 * 563
	m_xWindow = new sf::RenderWindow( sf::VideoMode{sf::Vector2u{1000, 563}}, "Accident Center 2", sf::State::Windowed ); // 1000 * 563
	ActionHandler::Instance().SetWindow(m_xWindow);

	sf::Cursor cursor {sf::Cursor::Type::Arrow};
	m_xWindow->setMouseCursor(cursor);

	auto& view = m_xSession.view;

	view = sf::View(
		/* center */ sf::Vector2f{ CFG().windowWidth * 0.5f, CFG().windowHeight * 0.5f },
		/* size   */ sf::Vector2f{ float(CFG().windowWidth), float(CFG().windowHeight) }
	);

	view.setViewport( sf::FloatRect{{0.f, 0.f}, {1.f, 1.f}} );
	m_xWindow->setView(view);

	m_xRender = new Render(*m_xWindow, m_xSession);
	m_xNetwork = new ClientNetworkSystem();
	m_xScreens = new ScreenManager(m_xSession);
}

ClientGame::~ClientGame() {
	delete m_xRender;
	delete m_xNetwork;
	delete m_xScreens;
}

void ClientGame::Run() {
/* // test

	GameTime ttglobalTime;
	myPlayer = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>();
	while(true) {

		ttglobalTime.Update();
		float dt = ttglobalTime.Delta();

		const auto& commands = myPlayer->GetController()->GenerateCommands(dt);
		for(const auto& com : commands) com->Execute();

		for(auto& pl : WProjectiles()) for(auto& cm : pl->GetController()->GenerateCommands(dt)) cm->Execute();

		//view.setCenter( utility::ClampToMap(myPlayer->Coord(), CFG().windowWidth, CFG().windowHeight, CFG().mapWidth, CFG().mapHeight) );
		ActionHandler::Instance().Clean();
		m_xRender->DrawWorld(dt, true);
	}
	return;
*/
	m_xScreens->Push( std::make_unique<MainMenuScreen>(*m_xWindow, *m_xScreens) );

	GameTime globalTime;
	while(true) {

		globalTime.Update();
		float dt = globalTime.Delta();

		auto* screen = m_xScreens->Current();
		while( auto event = m_xWindow->pollEvent() ) {

			if( event->is<sf::Event::Closed>() ) m_xWindow->close();
			if( screen ) screen->HandleEvent(*event);
		}

		auto* menu = dynamic_cast<MainMenuScreen*>(screen);
		if( menu && menu->IsStartGame() && !m_xNetwork->IsConnected() ) {
			ConectToServer();
			bGameSessionRunning = true;
			m_xScreens->Push( std::make_unique<GameScreen>(*m_xWindow, *m_xScreens) );
		}

		m_xWindow->clear();
		
		if( screen ) screen->Render(dt);

		if( bGameSessionRunning ) GameSessionRun(dt);

		m_xWindow->display();
	}


}

void ClientGame::ConectToServer() {
	m_xNetwork->Connect("185.30.202.137", 60000);
	//m_xNetwork->Connect("192.168.1.102", 60000);

	while( !m_xNetwork->IsConnected() || m_xNetwork->GetMyID()==0 ) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		m_xNetwork->Update();
	}
	
	m_xSession.m_xPlayer = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>();
	m_xSession.m_xPlayer->SetID(m_xNetwork->GetMyID());
	m_xSession.m_xPlayer->SetCoord( {350, 350} );
}

void ClientGame::GameSessionRun(float dt) {
	m_xNetwork->Update();

	m_xNetwork->UpdateInterpolation(dt);

	SendPlayerActivities(dt);

	ActionHandler::Instance().Clean();
	m_xRender->DrawWorld(dt, true);
}

void ClientGame::SendPlayerActivities(float dt) {
	auto& pl = m_xSession.m_xPlayer;
	const auto& commands = pl->GetController()->GenerateCommands(dt);

	for(auto& cmd : commands) {
		auto rawFireCommand = dynamic_cast<FireCommand*>(cmd.get());

		if( auto raw = dynamic_cast<MovementCommand*>(cmd.get()) ) {
			cmd->Execute();
		} else if( rawFireCommand ) {

			SoundMg().PlaySound("Resources/sound and music/shot.ogg");

			m_xNetwork->SendCommand( cmd.get() );
		}

	}

	m_xNetwork->SendPlayerState(m_xSession);
}