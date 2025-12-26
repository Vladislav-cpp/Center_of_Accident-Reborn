#pragma once
#include <unordered_map>
#include <unordered_set>
#include "StaticStatsProjectile.h"
#include "Projectile.h"
#include "CharacterFactory.h"
#include "NetworkCommon.h"
#include "World.h"
#include "StaticStatsUfo.h"


class ServerNetworkSystem : public net::tcp_server<MsgTypes>, public net::udp_server<MsgTypes> {
	public:
	ServerNetworkSystem(uint16_t nPort) : net::tcp_server<MsgTypes>(nPort), net::udp_server<MsgTypes>(nPort) {}

	void Start() {
		tcp_server::Start();
		udp_server::Start();
	}

	void Stop() {
		tcp_server::Stop();
		udp_server::Stop();
	}

	void Update(size_t nMaxMessages = -1, bool bWait = false) {	
		tcp_server::Update(nMaxMessages, bWait);
		udp_server::Update(nMaxMessages, bWait);
	}

	void OnTick() {
		++m_ServerTick;
	}

	WorldState m_sWorldState;
	uint32_t m_uProjID = 1100;
	uint32_t m_ServerTick = 0;

	std::mutex m_muxGarbage;
	std::vector<int> m_vGarbageIDs;

	virtual bool OnClientConnect(std::shared_ptr<net::tcpСonnection<MsgTypes>> client) override { 
		net::message<MsgTypes> msgAddOtherPlayers;
		msgAddOtherPlayers.header.id = MsgTypes::Client_Accepted;
		net::tcp_server<MsgTypes>::MessageClient(client, msgAddOtherPlayers);

		std::cout << "OnClientConnect " << std::endl; 

		return true; 
	}

	virtual void OnClientDisconnect(std::shared_ptr<net::tcpСonnection<MsgTypes>> client) override {
		std::lock_guard<std::mutex> lock(m_muxGarbage);
		m_vGarbageIDs.push_back( client->GetID() );
	}

	// udp and tcp 
	void OnMessage(const net::client_ref<MsgTypes>& client_ref, net::message<MsgTypes>& msg) override {

		auto tcpPtr = client_ref.tcp_ptr;
		auto udpPtr = client_ref.udp_ptr;
		
		switch (msg.header.id) {

		case MsgTypes::Client_RegisterWithServer :
		{
			
			PlayerDescription desc;

			auto id = tcpPtr->GetID();
			msg >> desc;
			desc.ID = id;
			m_sWorldState.PlayerRst.insert_or_assign(id, desc);

			std::cout << " Client_RegisterWithServer " << id << std::endl;

			net::message<MsgTypes> msgSendID;
			msgSendID.header.id = MsgTypes::Client_AssignID;
			msgSendID << id;
			net::tcp_server<MsgTypes>::MessageClient(tcpPtr, msgSendID);

			net::message<MsgTypes> msgAddPlayer;
			msgAddPlayer.header.id = MsgTypes::Game_AddPlayer;
			msgAddPlayer << desc;
			net::tcp_server<MsgTypes>::MessageAllClients(msgAddPlayer, tcpPtr);

			// IF NOT EXIST 
			auto pl = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>(-1);
			pl->SetID(id);
			std::cout << "createPlayer " << id << std::endl;

			// add new player oll already existing plsyers
			for(const auto& player : m_sWorldState.PlayerRst) {
				net::message<MsgTypes> msgAddOtherPlayers;
				msgAddOtherPlayers.header.id = MsgTypes::Game_AddPlayer;
				msgAddOtherPlayers << player.second;
				net::tcp_server<MsgTypes>::MessageClient( tcpPtr , msgAddOtherPlayers );
			}

			break;
		}

		case MsgTypes::Client_RegisterUDP : {
			std::cout << "Client_RegisterUDP - " ;

			uint32_t id;
			msg >> id;
			std::cout << id << std::endl;
			udpPtr->SetID(id);

			break;
		}

		case MsgTypes::Game_UpdatePlayer :
		{
			// Simply bounce update to everyone except incoming client
			//MessageAllClients(msg, client);

			PlayerDescription desc;
			msg >> desc;
			desc.ID = udpPtr->GetID();
			if(desc.ID == 0) std::cout << "id = 0\n";
			m_sWorldState.PlayerRst.insert_or_assign(desc.ID, desc);

			OnPlayerDataUpdate(desc.ID);
			break;
		}

		case MsgTypes::Game_AddProjectile: {
			ProjectileDescription desc;
			msg >> desc;

			// Генеруємо унікальний ID
			uint32_t projID = ++m_uProjID;
			desc.ID = projID;

			// Створюємо Projectile на сервері (координати беремо з desc)
			auto projectile = CharacterFactory::Instance().CreateAIPlayer<
				StaticStatsProjectile, Projectile>(
					projID, desc.PlayerPos, desc.MousePos 
			);

			projectile->SetID(desc.ID);
			projectile->SetOwnerID(desc.OwnerID);

			m_sWorldState.Projectiles.insert_or_assign(desc.ID, desc);

			// Відправляємо всім клієнтам
			//net::message<MsgTypes> broadcast;
			//broadcast.header.id = MsgTypes::Game_AddProjectile;
			//broadcast << desc;
			//MessageAllClients(broadcast);

			break;
		}

		}

	}

	// udp on Message
	void OnMessage(const std::shared_ptr<net::udpConnection<MsgTypes>> client, net::message<MsgTypes>& msg) override {}

	// tcp on Massage 
	void OnMessage(std::shared_ptr<net::tcpСonnection<MsgTypes>> client, net::message<MsgTypes>& msg) override {}

	void BroadcastWorld() {
		//OnSyncWorldState();

		net::message<MsgTypes> msg;
		msg.header.id = MsgTypes::Game_WorldUpdate;
	
		// if( !m_sWorldState.PlayerRst.empty() ) std::cout<< "BroadcastWorld Coord " << m_sWorldState.PlayerRst.begin()->second.m_vCoord.x << " - " << m_sWorldState.PlayerRst.begin()->second.m_vCoord.y << std::endl;
		PackList(msg, m_sWorldState.PlayerRst);
		PackList(msg, m_sWorldState.BotsRst);
		PackList(msg, m_sWorldState.Projectiles);
		//if(m_sWorldState.Projectiles.size()>0) std::cout << "m_sWorldState.Projectiles.size()>0 - " <<  m_sWorldState.Projectiles.size() << std::endl;

		msg << m_ServerTick;

		net::udp_server<MsgTypes>::MessageAll(msg);

	}

	void OnSyncWorldState() {
		// ---- 1. Синхронізація ботів / гравців ----

		// -----------------------------
		// 1. Видалити мертві BotsRst ID
		// -----------------------------
		{
			// Зібрати всі поточні ID
			std::unordered_set<uint32_t> aliveBots;
			for( auto& bot : WAIPlayers() ) aliveBots.insert(bot->ID());

			// Видалити з мапи те, чого немає у aliveBots
			std::erase_if(m_sWorldState.BotsRst, [&](const auto& pair) {
				return !aliveBots.contains(pair.first);
			});
		}

		// ------------------------------------------
		// 2. Видалити мертві Projectiles ID
		// ------------------------------------------
		{
			std::unordered_set<uint32_t> aliveProjectiles;
			for( auto& proj : WProjectiles() ) aliveProjectiles.insert( proj->ID() );

			std::erase_if(m_sWorldState.Projectiles, [&](const auto& pair) {
				return !aliveProjectiles.contains(pair.first);
			});
		}
		// ------------------------------------------
		// 3. Заново заповнити BotsRst
		// ------------------------------------------
		for(auto& bot : WAIPlayers()) {

			PlayerDescription desc;
			desc.ID      = bot->ID();
			desc.HP      = bot->HP();
			desc.m_vCoord = bot->Coord();

			m_sWorldState.BotsRst.insert_or_assign(desc.ID, desc);
		}

		// ------------------------------------------
		// 4. Заново заповнити Projectiles
		// ------------------------------------------
		//world.Projectiles.clear();

		for(auto& proj : WProjectiles()) {

			ProjectileDescription desc;
			desc.ID       = proj->ID();
			desc.OwnerID  = proj->GetOwnerID();
			//desc.MousePos = proj->GetMousePos();
			//desc.PlayerPos = proj->GetOwnerPlayerPos();
			desc.m_vCoord = proj->Coord();
			desc.Angle = proj->GetRotate();
			//std::cout << desc.Angle << std::endl;
			//std::cout << desc.ID << std::endl;

			m_sWorldState.Projectiles.insert_or_assign(desc.ID, desc);
		}

		// ------------------------------------------
		// 3. Заново заповнити PlayerRst
		// ------------------------------------------

		std::vector<int> idsToRemove;
		{
			std::lock_guard<std::mutex> lock(m_muxGarbage);
			if( !m_vGarbageIDs.empty() ) {
				idsToRemove = std::move(m_vGarbageIDs);
				m_vGarbageIDs.clear();
			}
		}

		if( !idsToRemove.empty() ) {
			// utility::KillByIDs(WHumanPlayers(), idsToRemove);
		}

		for(auto& pl : WHumanPlayers()) {

			PlayerDescription desc;
			desc.ID      = pl->ID();
			desc.HP      = pl->HP();
			desc.m_vCoord = pl->Coord();

			m_sWorldState.PlayerRst.insert_or_assign(desc.ID, desc);
		}
	}

	std::shared_ptr<PlayerCharacter> OnPlayerDataUpdate(int id = 0) {
		auto itState = m_sWorldState.PlayerRst.find(id);
		if( itState == m_sWorldState.PlayerRst.end() ) return nullptr;

		for( auto& player : WHumanPlayers() ) {

			if(player->ID() == id) {
				player->SetCoord(itState->second.m_vCoord);
				return player;
			}
		}

		return nullptr;

	}

	void SendResurrectPlayer(std::shared_ptr<PlayerCharacter> pl) {
		PlayerDescription outdata;
		outdata.ID = pl->ID();
		outdata.m_vCoord = pl->Coord();
		outdata.HP = pl->HP();

		net::message<MsgTypes> msg;
		msg.header.id = MsgTypes::Game_ResurrectPlayer;
		msg << outdata;

		net::tcp_server<MsgTypes>::MessageClient(pl->ID(), msg);
	}

	void SendEndInvulnerablePlayer(std::shared_ptr<PlayerCharacter> pl) {
		net::message<MsgTypes> msg;
		msg.header.id = MsgTypes::Game_EndtInvulnerable;

		net::tcp_server<MsgTypes>::MessageClient(pl->ID(), msg);
	}


};

