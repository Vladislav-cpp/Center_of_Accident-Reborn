#pragma once

#include <unordered_map>
#include "olc_net.h"
#include "NetworkCommon.h"
#include "Game.h"
#include "PlayerCharacter.h"
#include "CharacterFactory.h"
#include "AICharacter.h"
#include "StaticStatsProjectile.h"
#include "StaticStatsUfo.h"


class Client : public net::client_interface<MsgTypes> {
	public:
	Client() {
		//Connect("127.0.0.1", 60000); // !!
	}

	PlayerDescription descPlayer;
	uint32_t nPlayerID = 0;

	std::unordered_set<int> m_existingPlayerIDs;
	//std::unordered_map<uint32_t, PlayerDescription> mapObjects;

	WorldState m_sWorldState;

	bool OnClientUpdate(float fElapsedTime) {
		if( !IsConnected() ) return false;


		while( !Incoming().empty() ) {
			
			auto inMsg = Incoming().pop_front().msg;

			switch( inMsg.header.id ) { 
		
			case( MsgTypes::Client_Accepted ) : {
				net::message<MsgTypes> outMsg;
				outMsg.header.id = MsgTypes::Client_RegisterWithServer;
				descPlayer.m_vCoord = { 150, 150 };
				outMsg << descPlayer;
				Send(outMsg);
				
				std::cout << "Client_Accepted " << "\n";
				break;
			}

			case(MsgTypes::Client_AssignID):
			{
				// Server is assigning us OUR id
				inMsg >> nPlayerID;
				descPlayer.ID = nPlayerID;
				Game::Instance().createMainPlayer(nPlayerID);

				m_sWorldState.PlayerRst.insert_or_assign(nPlayerID, descPlayer);
				std::cout << "Assigned Client ID = " << nPlayerID << "\n";
				break;
			}

			case(MsgTypes::Game_AddPlayer):
			{
				inMsg >> descPlayer;
				const auto& ID = descPlayer.ID;
				if(ID == nPlayerID) break;

				auto [it, inserted] = m_sWorldState.PlayerRst.insert_or_assign(ID, descPlayer);

				if (m_existingPlayerIDs.find(ID) == m_existingPlayerIDs.end()) {
					m_existingPlayerIDs.insert(ID);

					Game::Instance().createPlayer(-1, ID);
					std::cout << " - createPlayer Game_AddPlayer - " << ID << "\n";
				} else {
					std::cout << "Player ID already exists: " << ID << "\n";
				}
				break;
			}

			case MsgTypes::Game_WorldUpdate:
			{
				m_sWorldState.BotsRst.clear();
				m_sWorldState.Projectiles.clear();
				m_sWorldState.PlayerRst.clear();

				UnpackList(inMsg, m_sWorldState.Projectiles);
				UnpackList(inMsg, m_sWorldState.BotsRst);
				UnpackList(inMsg, m_sWorldState.PlayerRst);

				break;
			}


			}
		
		}
		
	
	}

};

