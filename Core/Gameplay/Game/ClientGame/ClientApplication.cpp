#include "ClientApplication.h"

#include "GameController.h"
#include "ClientNetworkSystem.h"
#include "Render.h"
#include "ScreenManager.h"
#include "ActionHandler.h"
#include "Config.h"
#include "TimeSystem.h"

ClientApplication::ClientApplication() {
	Initialize();
}

ClientApplication::~ClientApplication() {
	delete m_mGame;
	delete m_mScreens;
	delete m_mRender;
	delete m_mNetwork;

	delete m_mSession;
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

	m_mSession	= new GameSession();
	m_mNetwork	= new ClientNetworkSystem(	*m_events );
	m_mRender	= new Render(				*m_window,	*m_session );
	m_mScreens	= new ScreenManager(		*m_window,	*m_events );
	m_mGame		= new GameController(		*m_network, *m_session );

	m_network->SetSession(m_session);

	sf::Cursor cursor{sf::Cursor::Type::Arrow};
	m_window->setMouseCursor(cursor);

	auto& view = m_session->view;

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

void ClientApplication::SubscribeToEvents() {

	m_mEvents->Subscribe<OnlineGameRequestedEvent>(
		[this](const OnlineGameRequestedEvent&) {
			OnOnlineGameRequested();
		}
	);

	m_mEvents->Subscribe<OfflineGameRequestedEvent>(
		[this](const OfflineGameRequestedEvent&) {
			OnOfflineGameRequested();
		}
	);

	m_mEvents->Subscribe<ReturnToInterfaceRequestedEvent>(
		[this](const ReturnToInterfaceRequestedEvent&) {
			OnReturnToInterfaceRequested();
		}
	);

	m_mEvents->Subscribe<ConnectionStateChangedEvent>(
		[this](const ConnectionStateChangedEvent& event) {
			OnConnectionStateChanged(event);
		}
	);
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

		if( event->is<sf::Event::Closed>() ) {

			m_mWindow->close(); 
			//TODO add save game state or ask user if he want save game state

			continue;
		}

		m_mScreens->HandleEvent(*event);

	}
}

void ClientApplication::Update(float dt) {

	switch(m_mMode) {
		case ApplicationMode::Interface:
			UpdateInterface(dt);
			break;

		case ApplicationMode::OnlineGame:
			UpdateOnlineGame(dt);
			break;

		case ApplicationMode::OfflineGame:
			UpdateOfflineGame(dt);
			break;
	}
}

void ClientApplication::UpdateInterface(float dt) {
}

void ClientApplication::UpdateOnlineGame(float dt) {

	m_mGame->UpdateOnline(dt);
}

void ClientApplication::UpdateOfflineGame(float dt) {

	m_mGame->UpdateOffline(dt);
}

void ClientApplication::Draw(float dt) {
	m_mWindow->clear();

	if(m_mMode == ApplicationMode::OnlineGame || m_mMode == ApplicationMode::OfflineGame) {
		m_mRender->DrawWorld(dt, true);
	}

	m_mScreens->Draw(dt);

	m_mWindow->display();
}

void ClientApplication::OnOnlineGameRequested()
{
	m_mNetwork->Connect(
		"192.168.1.102",
		60000
	);
}

void ClientApplication::OnOfflineGameRequested() {
	m_mMode = ApplicationMode::OfflineGame;
}

void ClientApplication::OnReturnToInterfaceRequested() {

	if(m_mMode == ApplicationMode::OnlineGame) {
		m_mNetwork->Disconnect();
	}

	m_mMode = ApplicationMode::Interface;
}

void ClientApplication::OnConnectionStateChanged( const ConnectionStateChangedEvent& event ) {

	if(event.newState == net::ConnectionState::Connected) {
		m_mMode = ApplicationMode::OnlineGame;
		return;
	}

	if(event.newState == net::ConnectionState::Failed) {
		m_mMode = ApplicationMode::Interface;
	}
}