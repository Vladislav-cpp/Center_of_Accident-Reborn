#pragma once
// implementation class //

void ScreenManager::Initialize() {
	m_mUIEvents = std::make_shared<EventBus>();

	sf::Cursor cursor{sf::Cursor::Type::Arrow};
	m_mWindow->setMouseCursor(cursor);

	m_mView = sf::View(
		{
			CFG().windowWidth * 0.5f,
			CFG().windowHeight * 0.5f
		},
		{
			float(CFG().windowWidth),
			float(CFG().windowHeight)
		}
	);

	m_mView.setViewport(
		sf::FloatRect{ {0.f, 0.f}, {1.f, 1.f} }
	);

	m_mWindow->setView(m_mView);

	Push( std::make_unique<GameScreen>(		*m_mWindow, m_mUIEvents) );
	Push( std::make_unique<GlobalOverlay>(	*m_mWindow, m_mUIEvents) );
	Push( std::make_unique<MainMenuScreen>(	*m_mWindow, m_mUIEvents) );

	GameScreen* gameScreen = dynamic_cast<GameScreen*>( m_xStack[ScreenSubType::GameScreen].get() );
	gameScreen->SetView( &m_mView );

	SubscribeToEvents();

	m_xCurrentScreens.push_back(ScreenSubType::MainMenu);
}

void ScreenManager::SubscribeToEvents() {

	m_mUIEvents->Subscribe<ButtonPressed>(
		[this](const ButtonPressed& event) {
			OnButtonPressed(event);
		}
	);

	// --- Model -> UI ---

	m_mModelEvents.Subscribe<OfflineGameStart>(
		[this](const OfflineGameStart& UIdata) {
			m_mUIEvents->Publish(HUDInitData{ UIdata.player });
		});

}

void ScreenManager::OnButtonPressed(const ButtonPressed& event) {

	switch (event.m_xType) {

		case UIType::MainMenuStartOffline: {
			// Handle start button press
			
			
			m_xCurrentScreens.clear();
			//TODO need m_xCurrentScreens.push_back(ScreenSubType::GlobalOverlay);
			// тут потрібно або на таймер підвязати GlobalOverlay і після нього при умові ( все завантажилось) запускати GameScreen;
			m_xCurrentScreens.push_back(ScreenSubType::GameScreen);

			m_mModelEvents.Publish<OfflineGameRequestedEvent>({});
			break;
		}

		case UIType::MainMenuOption: {
			 //Handle option button press
			break;
		}

		case UIType::MainMenuStartOnline: {

			m_xCurrentScreens.clear();
			//TODO need m_xCurrentScreens.push_back(ScreenSubType::GlobalOverlay);
			// тут потрібно або на таймер підвязати GlobalOverlay і після нього при умові ( все завантажилось) запускати GameScreen;
			m_xCurrentScreens.push_back(ScreenSubType::GameScreen);

			m_mModelEvents.Publish<OnlineGameRequestedEvent>({});
			break;
		}
		
		default: {
			// Handle other button presses
			break;
		}

	}

}



