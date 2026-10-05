#include "GameController.h"

#include "ClientNetworkSystem.h"
#include "GameSession.h"
#include "Render.h"

GameController::GameController( ClientNetworkSystem& network, GameSession& session )
	: m_mNetwork(network)
	, m_mSession(session)
{}

void GameController::UpdateOnline(float dt) {
	m_mNetwork.Update();
	m_mNetwork.UpdateInterpolation(dt);

}

void GameController::UpdateOffline(float dt) {
}