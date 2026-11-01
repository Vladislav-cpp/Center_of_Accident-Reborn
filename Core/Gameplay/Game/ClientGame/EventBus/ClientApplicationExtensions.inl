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

	m_mModelEvents->Subscribe<JoinAcceptedEvent>(
		[this](const JoinAcceptedEvent& event) {
			OnJoinAcceptedEvent(event);
		}
	);

	m_mModelEvents->Subscribe<MatchStateInf>(
		[this](const MatchStateInf& event) {
			OnMatchStateChanged(event);
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
	//if() TODO перевіряти m_mState->m_xNetData->state і треба нормально його оновлювати

	m_mNetwork->SendJoinRequest();
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

void ClientApplication::OnMatchStateChanged(const MatchStateInf& event){
	m_mState->m_xNetData->mstate = event.state;
	m_mState->m_xNetData->id = event.m_iMyID;


	if( event.state == MatchState::InProgress && m_mState->m_xNetData->state == PlayerConnState::Connecting ) {
		m_mState->mode = ApplicationMode::OnlineGame;
		
		m_mGame->InitializeOnline( event.m_iMyID );

		if(m_mState->m_xPlayer) m_mModelEvents->Publish<OnlineGameStart>({m_mState->m_xPlayer}); // for UI 
	}
}

void ClientApplication::OnJoinAcceptedEvent( const JoinAcceptedEvent& event ) {
		m_mState->m_xNetData->state = PlayerConnState::Connecting;
}