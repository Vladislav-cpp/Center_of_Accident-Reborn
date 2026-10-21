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

	ActionHandler::Instance().SetWindow(m_mWindow);

	m_mGameScreen		= new GameScreen(		*m_mWindow, m_mUIEvents);
	m_mGlobalOverlay	= new GlobalOverlay(	*m_mWindow, m_mUIEvents);
	m_mMainMenuScreen	= new MainMenuScreen(	*m_mWindow, m_mUIEvents);

	SubscribeToEvents();
}

void ScreenManager::SubscribeToEvents() {

	m_mUIEvents->Subscribe<ButtonPressed>(
		[this](const ButtonPressed& event) {
			OnButtonPressed(event);
		}
	);

}

void ScreenManager::OnButtonPressed(const ButtonPressed& event) {

	switch (event.m_xType) {

		case UIType::MainMenuStart: {
			// Handle start button press
			Pop();
			
			//Push();
			m_mModelEvents.Publish<OnlineGameRequestedEvent>({});
			break;
		}

		//case UIType::MainMenuOption: {
			// Handle option button press
		//	break;
		//}

		//case UIType::Back: {
		//	Pop();
		//	break;
		//}
		
		default: {
			// Handle other button presses
			break;
		}

	}

}



