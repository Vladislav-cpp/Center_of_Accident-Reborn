#pragma once
#include <unordered_map>
#include "StaticStatsProjectile.h"
#include "Projectile.h"
#include "CharacterFactory.h"
#include "NetworkCommon.h"
#include "World.h"
#include "StaticStatsUfo.h"


class ServerNetworkSystem : public net::server_interface<MsgTypes> {
	public:
	ServerNetworkSystem(uint16_t nPort) : net::server_interface<MsgTypes>(nPort) {}

	WorldState m_sWorldState;
	uint32_t m_uProjID = 1100;
//	std::vector<uint32_t> m_vGarbageIDs;

	virtual bool OnClientConnect(std::shared_ptr<net::connection<MsgTypes>> client) override { 
		net::message<MsgTypes> msgAddOtherPlayers;
		msgAddOtherPlayers.header.id = MsgTypes::Client_Accepted;
		MessageClient(client, msgAddOtherPlayers);

		std::cout << "OnClientConnect " << std::endl;

		return true; 
	}

	void OnMessage(std::shared_ptr<net::connection<MsgTypes>> client, net::message<MsgTypes>& msg) override {
		//if (!m_vGarbageIDs.empty())
		//{
		//	for (auto pid : m_vGarbageIDs)
		//	{
		//		net::message<MsgTypes> m;
		//		m.header.id = MsgTypes::Game_RemovePlayer;
		//		m << pid;
		//		std::cout << "Removing " << pid << "\n";
		//		MessageAllClients(m);
		//	}
		//	m_vGarbageIDs.clear();
		//}



		switch (msg.header.id) {

		case MsgTypes::Client_RegisterWithServer :
		{
			PlayerDescription desc;

			auto id = client->GetID();
			msg >> desc;
			desc.ID = id;
			m_sWorldState.PlayerRst.insert_or_assign(id, desc);

			net::message<MsgTypes> msgSendID;
			msgSendID.header.id = MsgTypes::Client_AssignID;
			msgSendID << id;
			MessageClient(client, msgSendID);

			net::message<MsgTypes> msgAddPlayer;
			msgAddPlayer.header.id = MsgTypes::Game_AddPlayer;
			msgAddPlayer << desc;
			MessageAllClients(msgAddPlayer, client);

			// IF NOT EXIST 
			auto pl = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>(-1);
			pl->SetID(id);
			std::cout << "createPlayer " << id << std::endl;

			// add new player oll already existing plsyers
			for(const auto& player : m_sWorldState.PlayerRst) {
				net::message<MsgTypes> msgAddOtherPlayers;
				msgAddOtherPlayers.header.id = MsgTypes::Game_AddPlayer;
				msgAddOtherPlayers << player.second;
				MessageClient( client , msgAddOtherPlayers );
			}

			break;
		}


		case MsgTypes::Game_UpdatePlayer :
		{
			// Simply bounce update to everyone except incoming client
			//MessageAllClients(msg, client);

			PlayerDescription desc;
			msg >> desc;
			desc.ID = client->GetID();
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

	void BroadcastWorld() {
		//OnSyncWorldState();

		net::message<MsgTypes> msg;
		msg.header.id = MsgTypes::Game_WorldUpdate;

		
		// if( !m_sWorldState.PlayerRst.empty() ) std::cout<< "BroadcastWorld Coord " << m_sWorldState.PlayerRst.begin()->second.m_vCoord.x << " - " << m_sWorldState.PlayerRst.begin()->second.m_vCoord.y << std::endl;
		PackList(msg, m_sWorldState.PlayerRst);
		PackList(msg, m_sWorldState.BotsRst);
		PackList(msg, m_sWorldState.Projectiles);
		//if(m_sWorldState.Projectiles.size()>0) std::cout << "m_sWorldState.Projectiles.size()>0 - " <<  m_sWorldState.Projectiles.size() << std::endl;

		MessageAllClients(msg);

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
		for(auto& pl : WHumanPlayers()) {

			PlayerDescription desc;
			desc.ID      = pl->ID();
			desc.HP      = pl->HP();
			desc.m_vCoord = pl->Coord();

			m_sWorldState.PlayerRst.insert_or_assign(desc.ID, desc);
		}
	}

	void OnPlayerDataUpdate(int id = 0) {
		auto itState = m_sWorldState.PlayerRst.find(id);
		if( itState == m_sWorldState.PlayerRst.end() ) return;

		for( auto& player : WHumanPlayers() ) {

			if (player->ID() == id) {
				player->SetCoord(itState->second.m_vCoord);
				return;
			}
		}

	}

};

