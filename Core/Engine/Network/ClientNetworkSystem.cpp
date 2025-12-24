#include "ClientNetworkSystem.h"
#include "World.h"
#include "PlayerCharacter.h"
#include "AICharacter.h"
#include "Projectile.h"
#include "CharacterFactory.h"
#include "StaticStatsDust.h"
#include "StaticStatsUfo.h"
#include "StaticStatsProjectile.h"
#include "GameSession.h"
#include "FireCommand.h"

bool ClientNetworkSystem::Connect(const std::string& host, const uint16_t port) {

	auto bConnect_tcp = net::tcp_client<MsgTypes>::Connect(host, port);
	auto bStart_udp = net::udp_client<MsgTypes>::Start(host, port);
	return bConnect_tcp && bStart_udp;
}

void ClientNetworkSystem::Update() {
	if( !IsConnected() ) return;

	Update_TCP();
	Update_UDP();
}

void ClientNetworkSystem::Update_TCP() {
	auto& tcp_inComing = net::tcp_client<MsgTypes>::Incoming();

while( !tcp_inComing.empty() ) {
			
	auto tcp_inMsg = !tcp_inComing.empty() ? tcp_inComing.pop_front().msg : net::message<MsgTypes>();

	switch( tcp_inMsg.header.id ) { 
		
	case( MsgTypes::Client_Accepted ) : {
		net::message<MsgTypes> outMsg;
		outMsg.header.id = MsgTypes::Client_RegisterWithServer;
		myDescription.m_vCoord = { 150, 150 };
		outMsg << myDescription;
		net::tcp_client<MsgTypes>::Send(outMsg);
				
		std::cout << "Client_Accepted " << "\n";
		break;
	}

	case(MsgTypes::Client_AssignID) : {
		// Server is assigning us OUR id
		tcp_inMsg >> m_iMyID;
		myDescription.ID = m_iMyID;

		//m_sWorldState.PlayerRst.insert_or_assign(m_iMyID, myDescription);
		std::cout << "Assigned Client ID = " << m_iMyID << "\n";

		net::message<MsgTypes> udpRegisterMsg;
		udpRegisterMsg.header.id = MsgTypes::Client_RegisterUDP;

		udpRegisterMsg << m_iMyID;
		net::udp_client<MsgTypes>::Send(udpRegisterMsg);

		std::cout << "Sent UDP registration for ID: " << m_iMyID << "\n";
		break;
	}

	case(MsgTypes::Game_AddPlayer) : {
		PlayerDescription otherDesc;
		tcp_inMsg >> otherDesc;
		const auto& ID = otherDesc.ID;
		if(ID == m_iMyID) break;

		//auto [it, inserted] = m_sWorldState.PlayerRst.insert_or_assign(ID, otherDesc);

		if(m_existingPlayerIDs.find(ID) == m_existingPlayerIDs.end()) {
			m_existingPlayerIDs.insert(ID);

			auto pl = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>(-1);
			pl->SetID(ID);

			std::cout << " - createPlayer Game_AddPlayer - " << ID << "\n";
		} else {
			std::cout << "Player ID already exists: " << ID << "\n";
		}
		break;
	}

	} // switch
} // while

}

void ClientNetworkSystem::Update_UDP() {
	auto& udp_inComing = net::udp_client<MsgTypes>::Incoming();

while( !udp_inComing.empty() ) {
			
	auto udp_inMsg = !udp_inComing.empty() ? udp_inComing.pop_front().msg : net::message<MsgTypes>();


	switch( udp_inMsg.header.id ) { 

	case MsgTypes::Game_WorldUpdate : {
		WorldSnapshot newSnapshot;

		//m_sWorldState.BotsRst.clear();
		//m_sWorldState.Projectiles.clear();
		//m_sWorldState.PlayerRst.clear();

		udp_inMsg >> newSnapshot.ServerTick;

		UnpackList(udp_inMsg, newSnapshot.WState.Projectiles);
		UnpackList(udp_inMsg, newSnapshot.WState.BotsRst);
		UnpackList(udp_inMsg, newSnapshot.WState.PlayerRst);


		m_snapshotBuffer.push_back(newSnapshot);

		if(m_snapshotBuffer.size() > MAX_BUFFER_SIZE) {
			m_snapshotBuffer.pop_front();
		}
		
		break;
	}

	} // switch
} // while

}

void ClientNetworkSystem::SendPlayerState(const GameSession& seddion) {

	PlayerDescription outdata;
	outdata.ID = GetMyID();
	outdata.m_vCoord = seddion.m_xPlayer->Coord();
	//outdata.HP = player->HP();

	net::message<MsgTypes> msg;
	msg.header.id = MsgTypes::Game_UpdatePlayer;
	msg << outdata;

	net::udp_client<MsgTypes>::Send(msg);
}

void ClientNetworkSystem::SendCommand(Command* comm) {
	if( auto* rawFireCommand = dynamic_cast<FireCommand*>(comm) ) {

		ProjectileDescription desc;
		auto pl = rawFireCommand->Object();

		desc.OwnerID = pl->ID();
		desc.MousePos = rawFireCommand->GetMousePosition();
		desc.PlayerPos = pl->Coord();

		net::message<MsgTypes> msg;
		msg.header.id = MsgTypes::Game_AddProjectile;
		msg << desc;
			
		net::tcp_client<MsgTypes>::Send(msg);
	}

}

void ClientNetworkSystem::UpdateInterpolation(float dt) {
if (m_snapshotBuffer.size() < 2) return;

    // 1. Оновлюємо внутрішній час клієнта
    // Ми хочемо показувати стан, який був 100мс тому
    m_fCurrentInterpolationTime += dt;

    // Встановлюємо цільовий тік для рендерингу
    // Наприклад: останній отриманий тік мінус затримка
    float targetTick = (float)m_snapshotBuffer.back().ServerTick - (m_fInterpolationDelay * 60.0f);

    // 2. Шукаємо два знімки, між якими потрапляє targetTick
    WorldSnapshot* pA = nullptr;
    WorldSnapshot* pB = nullptr;

    for (size_t i = 0; i < m_snapshotBuffer.size() - 1; ++i) {
        if (targetTick >= m_snapshotBuffer[i].ServerTick && targetTick < m_snapshotBuffer[i+1].ServerTick) {
            pA = &m_snapshotBuffer[i];
            pB = &m_snapshotBuffer[i+1];
            break;
        }
    }

    if (pA && pB) {
        // 3. Рахуємо коефіцієнт інтерполяції (t від 0 до 1)
        float t = (targetTick - (float)pA->ServerTick) / ((float)pB->ServerTick - (float)pA->ServerTick);

        // 4. Оновлюємо візуальні об'єкти
        InterpolateEntities(pA->WState, pB->WState, t);
    }
}

void ClientNetworkSystem::InterpolateEntities(const WorldState& stateA, const WorldState& stateB, float t) {

	WorldState worldState;

	for(auto& [id, botB] : stateB.BotsRst) {
		// Шукаємо цього ж бота в знімку A (минуле)

		if( !stateA.BotsRst.contains(id) ) continue;

		PlayerDescription newDesc;

		const auto& botA = stateA.BotsRst.at(id);

		// ЛЕРПИМО ПОЗИЦІЮ!
		sf::Vector2f interpolatedPos = botA.m_vCoord + t * (botB.m_vCoord - botA.m_vCoord);

		// Оновлюємо візуальний спрайт бота в грі

		newDesc.HP = botB.HP;
		newDesc.ID = id;
		newDesc.m_vCoord = interpolatedPos;
	
		worldState.BotsRst.insert( {id , newDesc} );
	}

	for(auto& [id, plB] : stateB.PlayerRst) {
		// Шукаємо цього ж бота в знімку A (минуле)

		if( !stateA.PlayerRst.contains(id) ) continue;

		PlayerDescription newDesc;

		const auto& plA = stateA.PlayerRst.at(id);

		// ЛЕРПИМО ПОЗИЦІЮ!
		sf::Vector2f interpolatedPos = plA.m_vCoord + t * (plB.m_vCoord - plA.m_vCoord);

		// Оновлюємо візуальний спрайт бота в грі

		newDesc.HP = plB.HP;
		newDesc.ID = id;
		newDesc.m_vCoord = interpolatedPos;
	
		worldState.PlayerRst.insert( {id , newDesc} );
	}

	for(auto& [id, pjB] : stateB.Projectiles) {
		// Шукаємо цього ж бота в знімку A (минуле)

		if( !stateA.Projectiles.contains(id) ) continue;

		ProjectileDescription newDesc;

		const auto& pjA = stateA.Projectiles.at(id);

		// ЛЕРПИМО ПОЗИЦІЮ!
		sf::Vector2f interpolatedPos = pjA.m_vCoord + t * (pjB.m_vCoord - pjA.m_vCoord);

		// Оновлюємо візуальний спрайт бота в грі

		newDesc.OwnerID = pjB.OwnerID;
		newDesc.ID = id;
		newDesc.Angle = pjB.Angle;
		newDesc.m_vCoord = interpolatedPos;
	
		worldState.Projectiles.insert( {id , newDesc} );
	}

	WorldUpdate(worldState);
	
}

void ClientNetworkSystem::WorldUpdate(const WorldState& worldState) {
	//if(player==nullptr) return;

	// === 1. Видаляємо об’єкти, яких нема в мапах ===
	kill_if_not_in_worldstate( WAllObjects(), worldState, m_iMyID );

	kill_if_not_in_map( WAIPlayers(), worldState.BotsRst );
	kill_if_not_in_map( WProjectiles(), worldState.Projectiles );


	// === 2. Оновлюємо координати існуючих ===

	// Гравці
	for(auto& [id, st] : worldState.PlayerRst) {	

		auto it = std::find_if(WHumanPlayers().begin(), WHumanPlayers().end(),
			[&](const auto& o){ return o->ID() == id; });

		if( it != WHumanPlayers().end() ) {

			(*it)->SetHP(st.HP);

			if(m_iMyID == id) continue;
			(*it)->SetCoord(st.m_vCoord);
		}
	}

	// Боти
	for (auto& [id, st] : worldState.BotsRst) {
		auto it = std::find_if(WAIPlayers().begin(), WAIPlayers().end(),
			[&](const auto& o){ return o->ID() == id; });

		if (it != WAIPlayers().end())
		{
			(*it)->SetCoord(st.m_vCoord);
			(*it)->SetHP(st.HP);
		}
	}

	// пулі
	for (auto& [id, st] : worldState.Projectiles)
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
		worldState.BotsRst,
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
		worldState.PlayerRst,
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
		worldState.Projectiles,
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