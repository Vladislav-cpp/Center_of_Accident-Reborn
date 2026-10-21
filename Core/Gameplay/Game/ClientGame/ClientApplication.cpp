#include "ClientApplication.h"

#include "GameController.h"
#include "ClientNetworkSystem.h"
#include "Render.h"
#include "ScreenManager.h"
#include "ActionHandler.h"
#include "Config.h"
#include "TimeSystem.h"
#include "EventBus.h"
#include "GameState.h"

ClientApplication::ClientApplication() {
	Initialize();
}

ClientApplication::~ClientApplication() {
	delete m_mGame;
	delete m_mScreens;
	delete m_mNetwork;
	delete m_mState;
	delete m_mModelEvents;
	delete m_mWindow;
}

void ClientApplication::Initialize() {
	m_mModelEvents = new EventBus();

	m_mState	= new GameState(); 
	m_mNetwork	= new ClientNetworkSystem(	*m_mModelEvents );
	m_mScreens	= new ScreenManager(		m_mWindow, *m_mModelEvents );
	m_mGame		= new GameController(		*m_mModelEvents );

	CFG().windowWidth = 1000;
	CFG().windowHeight = 563;

	m_mWindow = new sf::RenderWindow(
		sf::VideoMode({1000, 563}),
		"Accident Center 2",
		sf::State::Windowed
	);

	m_mScreens->Initialize();

	SubscribeToEvents();
}

void ClientApplication::Run() {

	while( m_mWindow->isOpen() ) {

		TimeSys().Tick();

		const float dt = TimeSys().DeltaTime();

		ProcessEvents();

		Update(dt);

		Draw(dt);
		

		//TODO we need can be left and need have limit FPS
	}
}

void ClientApplication::ProcessEvents() {

	while( auto event = m_mWindow->pollEvent() ) {

		// перевірка на закриття вікна
		if( event->is<sf::Event::Closed>() ) {

			m_mWindow->close(); //TODO тут треба закрити всі підсистеми, а не тільки вікно і закрити процес 
			//TODO add save game state or ask user if he want save game state

			break;
		}

		// перевіряємо чи це натиск на UI
		// перевірити чи наша програма в фокусі
		if(m_mScreens->HandleEvent(*event)) {
			continue;
		}

		// якщо це не натиск на UI, то дивимось хто в нас щє приймає клікі
		
		

	}
}

void ClientApplication::Draw(float dt) {
	m_mWindow->clear();

	if(m_mState->mode == ApplicationMode::OnlineGame || m_mState->mode == ApplicationMode::OfflineGame) {
		m_mRender->DrawWorld(dt, true);
	}

	m_mScreens->Draw(dt);

	m_mWindow->display();
}

void ClientApplication::UpdateInterface(float dt) {

}

void ClientApplication::UpdateOnlineGame(float dt) {

	m_mGame->UpdateOnline(dt);
}

void ClientApplication::UpdateOfflineGame(float dt) {

	m_mGame->UpdateOffline(dt);
}

#include "ClientApplicationExtensions.inl"