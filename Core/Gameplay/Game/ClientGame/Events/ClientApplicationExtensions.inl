#pragma once
// implementation include //
#include <ClientEvents.h>
#include <ClientNetworkSystem.h>

void ClientApplication::SubscribeToEvents() {

	m_mEvents->Subscribe<OnlineGameRequestedEvent>(
		[this](const OnlineGameRequestedEvent& event) {
			OnOnlineGame(event);
		}
	);

	m_mEvents->Subscribe<OfflineGameRequestedEvent>(
		[this](const OfflineGameRequestedEvent& event) {
			OnOfflineGame(event);
		}
	);

	m_mEvents->Subscribe<ReturnToInterfaceRequestedEvent>(
		[this](const ReturnToInterfaceRequestedEvent& event) {
			OnReturnToInterface(event);
		}
	);

	m_mEvents->Subscribe<ConnectionStateChangedEvent>(
		[this](const ConnectionStateChangedEvent& event) {
			OnConnectionStateChanged(event);
		}
	);
}

void ClientApplication::Update(float dt) {

	switch(m_mState->mode) {
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

void ClientApplication::OnOnlineGame(const OnlineGameRequestedEvent& event) {
	m_mNetwork->Connect(
		"192.168.1.102",
		60000
	);
}	

void ClientApplication::OnOfflineGame(const OfflineGameRequestedEvent& event) {
	m_mState->mode = ApplicationMode::OfflineGame;
}

void ClientApplication::OnReturnToInterface(const ReturnToInterfaceRequestedEvent& event) {

	if(m_mState->mode == ApplicationMode::OnlineGame) {
		//m_mNetwork->Disconnect();
	}

	m_mState->mode = ApplicationMode::Interface;
}

void ClientApplication::OnConnectionStateChanged( const ConnectionStateChangedEvent& event ) {

	//if(event.newState == net::ConnectionState::Connected) {
	//	m_mState->mode = ApplicationMode::OnlineGame;
	//	return;
	//}

	//if(event.newState == net::ConnectionState::Failed) {
	//	m_mState->mode = ApplicationMode::Interface;
	//}
}