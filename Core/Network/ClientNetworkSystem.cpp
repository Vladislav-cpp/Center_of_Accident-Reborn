#include "ClientNetworkSystem.h"
#include "World.h"
#include "PlayerCharacter.h"
#include "AICharacter.h"
#include "Projectile.h"
#include "CharacterFactory.h"
#include "StaticStatsDust.h"
#include "StaticStatsUfo.h"
#include "StaticStatsProjectile.h"

void ClientNetworkSystem::Update() {

if( !IsConnected() ) return;


while( !Incoming().empty() ) {
			
	auto inMsg = Incoming().pop_front().msg;

	switch( inMsg.header.id ) { 
		
	case( MsgTypes::Client_Accepted ) : {
		net::message<MsgTypes> outMsg;
		outMsg.header.id = MsgTypes::Client_RegisterWithServer;
		myDescription.m_vCoord = { 150, 150 };
		outMsg << myDescription;
		Send(outMsg);
				
		std::cout << "Client_Accepted " << "\n";
		break;
	}

	case(MsgTypes::Client_AssignID) : {
		// Server is assigning us OUR id
		inMsg >> m_iMyID;
		myDescription.ID = m_iMyID;
		//Game::Instance().createMainPlayer(m_iMyID); ++++++++++

		m_sWorldState.PlayerRst.insert_or_assign(m_iMyID, myDescription);
		std::cout << "Assigned Client ID = " << m_iMyID << "\n";
		break;
	}

	case(MsgTypes::Game_AddPlayer) : {
		PlayerDescription otherDesc;
		inMsg >> otherDesc;
		const auto& ID = otherDesc.ID;
		if(ID == m_iMyID) break;

		auto [it, inserted] = m_sWorldState.PlayerRst.insert_or_assign(ID, otherDesc);

		if (m_existingPlayerIDs.find(ID) == m_existingPlayerIDs.end()) {
			m_existingPlayerIDs.insert(ID);

			auto pl = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>(-1);
			pl->SetID(ID);

			std::cout << " - createPlayer Game_AddPlayer - " << ID << "\n";
		} else {
			std::cout << "Player ID already exists: " << ID << "\n";
		}
		break;
	}

	case MsgTypes::Game_WorldUpdate : {
		m_sWorldState.BotsRst.clear();
		m_sWorldState.Projectiles.clear();
		m_sWorldState.PlayerRst.clear();

		UnpackList(inMsg, m_sWorldState.Projectiles);
		UnpackList(inMsg, m_sWorldState.BotsRst);
		UnpackList(inMsg, m_sWorldState.PlayerRst);
		
		OnWorldUpdate();
		break;
	}


	} // switch
} // while
}

void ClientNetworkSystem::OnWorldUpdate() {
	//if(player==nullptr) return;

	// === 1. Видаляємо об’єкти, яких нема в мапах ===
	kill_if_not_in_worldstate( WAllObjects(), m_sWorldState, m_iMyID );

	kill_if_not_in_map( WAIPlayers(), m_sWorldState.BotsRst );
	kill_if_not_in_map( WProjectiles(), m_sWorldState.Projectiles );


	// === 2. Оновлюємо координати існуючих ===

	// Гравці
	for(auto& [id, st] : m_sWorldState.PlayerRst) {	

		auto it = std::find_if(WHumanPlayers().begin(), WHumanPlayers().end(),
			[&](const auto& o){ return o->ID() == id; });

		if( it != WHumanPlayers().end() ) {

			(*it)->SetHP(st.HP);

			if(m_iMyID == id) continue;
			(*it)->SetCoord(st.m_vCoord);
		}
	}

	// Боти
	for (auto& [id, st] : m_sWorldState.BotsRst) {
		auto it = std::find_if(WAIPlayers().begin(), WAIPlayers().end(),
			[&](const auto& o){ return o->ID() == id; });

		if (it != WAIPlayers().end())
		{
			(*it)->SetCoord(st.m_vCoord);
			(*it)->SetHP(st.HP);
		}
	}

	// пулі
	for (auto& [id, st] : m_sWorldState.Projectiles)
	{
		auto it = std::find_if(WProjectiles().begin(), WProjectiles().end(),
			[&](const auto& o){ return o->ID() == id; });

		if (it != WProjectiles().end())
		{
			(*it)->SetCoord(st.m_vCoord);
			(*it)->SetRotate(st.Angle);
		}
	}


	// === 3. Створюємо ті, яких не було ===
	create_missing(
		WAIPlayers(),
		m_sWorldState.BotsRst,
		[&](int id, const auto& st){
			auto obj = CharacterFactory::Instance().CreateAIPlayer<StaticStatsDust>();
			obj->SetCoord(st.m_vCoord);
			obj->SetHP(st.HP);
			obj->SetID(st.ID);
			return obj;
		}
	);

	create_missing(
		WHumanPlayers(),
		m_sWorldState.PlayerRst,
		[&](int id, const auto& st){
			auto obj = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>(-1);
			obj->SetCoord(st.m_vCoord);
			obj->SetID(st.ID);
			obj->SetHP(st.HP);
			return obj;
		},
		m_iMyID
	);

	create_missing(
		WProjectiles(),
		m_sWorldState.Projectiles,
		[&](int id, const auto& st){
			auto obj = CharacterFactory::Instance().CreateAIPlayer<StaticStatsProjectile, Projectile>(-1, sf::Vector2f{0, 0}, sf::Vector2f{0, 0});
			obj->SetCoord(st.m_vCoord);
			obj->SetID(st.ID);
			obj->SetOwnerID(st.OwnerID);
			obj->SetRotate(st.Angle);
			return obj;
		}
	);

}