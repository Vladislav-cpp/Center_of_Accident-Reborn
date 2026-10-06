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
	delete m_mRender;
	delete m_mNetwork;

	delete m_mState;
	delete m_mWindow;

	delete m_mEvents;
}

void ClientApplication::Initialize() {
	CFG().windowWidth = 1000;
	CFG().windowHeight = 563;

	m_mEvents = new EventBus();

	m_mWindow = new sf::RenderWindow(
		sf::VideoMode({1000, 563}),
		"Accident Center 2",
		sf::State::Windowed
	);

	m_mState	= new GameState(); 
	m_mNetwork	= new ClientNetworkSystem(	*m_mEvents );
	m_mRender	= new Render(				*m_mWindow );
	m_mScreens	= new ScreenManager(		*m_mEvents );
	m_mGame		= new GameController(		*m_mEvents, *m_mNetwork);

	sf::Cursor cursor{sf::Cursor::Type::Arrow};
	m_mWindow->setMouseCursor(cursor);

	auto& view = m_mState->view;

	view = sf::View(
		{
			CFG().windowWidth * 0.5f,
			CFG().windowHeight * 0.5f
		},
		{
			float(CFG().windowWidth),
			float(CFG().windowHeight)
		}
	);

	view.setViewport(
		sf::FloatRect{ {0.f, 0.f}, {1.f, 1.f} }
	);

	m_mWindow->setView(view);

	ActionHandler::Instance().SetWindow(m_mWindow);

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

			continue;
		}

		// перевіряємо чи це натиск на UI
		if(m_mScreens->HandleEvent(*event)) {
			continue;
		}
		
		

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

#include "ClientApplicationExtensions.inl"

void ClientApplication::UpdateInterface(float dt) {
}

void ClientApplication::UpdateOnlineGame(float dt) {

	m_mGame->UpdateOnline(dt);
}

void ClientApplication::UpdateOfflineGame(float dt) {

	m_mGame->UpdateOffline(dt);
}

/*
void ClientApplication::OnOnlineGameRequested()
{
	m_mNetwork->Connect(
		"192.168.1.102",
		60000
	);
}

void ClientApplication::OnOfflineGameRequested() {
	m_mState->mode = ApplicationMode::OfflineGame;
}

void ClientApplication::OnReturnToInterfaceRequested() {

	if(m_mState->mode == ApplicationMode::OnlineGame) {
		m_mNetwork->Disconnect();
	}

	m_mState->mode = ApplicationMode::Interface;
}

void ClientApplication::OnConnectionStateChanged( const ConnectionStateChangedEvent& event ) {

	if(event.newState == net::ConnectionState::Connected) {
		m_mState->mode = ApplicationMode::OnlineGame;
		return;
	}

	if(event.newState == net::ConnectionState::Failed) {
		m_mState->mode = ApplicationMode::Interface;
	}
}
*/