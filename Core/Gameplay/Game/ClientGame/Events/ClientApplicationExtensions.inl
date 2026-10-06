// implementation include //
#include <ClientEvents.h>

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