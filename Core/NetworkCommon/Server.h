#pragma once
#include <unordered_map>
#include "Game.h"
#include "StaticStatsProjectile.h"
#include "Projectile.h"
#include "CharacterFactory.h"
#include "NetworkCommon.h"


class Server : public net::server_interface<MsgTypes> {
	public:
	Server(uint16_t nPort) : net::server_interface<MsgTypes>(nPort) {}

	WorldState m_sWorldState;
	uint32_t m_uProjID = 0;
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
			Game::Instance().createPlayer(-1, id); // !!!!!
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
		net::message<MsgTypes> msg;
		msg.header.id = MsgTypes::Game_WorldUpdate;

		
		// if( !m_sWorldState.PlayerRst.empty() ) std::cout<< "BroadcastWorld Coord " << m_sWorldState.PlayerRst.begin()->second.m_vCoord.x << " - " << m_sWorldState.PlayerRst.begin()->second.m_vCoord.y << std::endl;
		PackList(msg, m_sWorldState.PlayerRst);
		PackList(msg, m_sWorldState.BotsRst);
		PackList(msg, m_sWorldState.Projectiles);
		//if(m_sWorldState.Projectiles.size()>0) std::cout << "m_sWorldState.Projectiles.size()>0 - " <<  m_sWorldState.Projectiles.size() << std::endl;

		MessageAllClients(msg);

	}

};

