#pragma once
// implementation include //
#include <ClientEvents.h>
#include <ClientNetworkSystem.h>

void ClientApplication::SubscribeToEvents() {

	m_mModelEvents->Subscribe<OnlineGameRequestedEvent>(
		[this](const OnlineGameRequestedEvent& event) {
			OnOnlineGame(event);
		}
	);

	m_mModelEvents->Subscribe<OfflineGameRequestedEvent>(
		[this](const OfflineGameRequestedEvent& event) {
			OnOfflineGame(event);
		}
	);

	m_mModelEvents->Subscribe<ReturnToInterfaceRequestedEvent>(
		[this](const ReturnToInterfaceRequestedEvent& event) {
			OnReturnToInterface(event);
		}
	);

	m_mModelEvents->Subscribe<ConnectionStateChangedEvent>(
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
		"192.168.1.102", //TODO брати з функції яка буде дивитись в Config але знатиме чи це локалка чи сервер
						 //TODO тут щє треба резолвити повторне перепідключення  -  в випадку якщо не можемопідключитись викидати в мейн меню
		60000
	);

	m_mState->mode = ApplicationMode::OnlineGame; //TODO це треба ставити не тут а в m_mNetwork а тут вже отримувати відповідь і рухати далі

	m_mGame->InitializeOnline();

	if(m_mState->m_xPlayer) m_mModelEvents->Publish<OnlineGameStart>({m_mState->m_xPlayer});
}

void ClientApplication::OnOfflineGame(const OfflineGameRequestedEvent& event) {
	m_mState->mode = ApplicationMode::OfflineGame;

	m_mGame->InitializeOffline();

	if(m_mState->m_xPlayer) m_mModelEvents->Publish<OfflineGameStart>({m_mState->m_xPlayer}); // for UI 
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