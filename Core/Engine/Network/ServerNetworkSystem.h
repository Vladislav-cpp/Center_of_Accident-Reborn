#pragma once
#include <unordered_map>
#include "StaticStatsProjectile.h"
#include "CharacterFactory.h"
#include "NetworkCommon.h"


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

	virtual bool OnClientConnect(std::shared_ptr<net::tcpÑonnection<MsgTypes>> client) override;

	virtual void OnClientDisconnect(std::shared_ptr<net::tcpÑonnection<MsgTypes>> client) override;

	// udp and tcp 
	void OnMessage(const net::client_ref<MsgTypes>& client_ref, net::message<MsgTypes>& msg) override;

	// udp on Message
	void OnMessage(const std::shared_ptr<net::udpConnection<MsgTypes>> client, net::message<MsgTypes>& msg) override {}

	// tcp on Massage 
	void OnMessage(std::shared_ptr<net::tcpÑonnection<MsgTypes>> client, net::message<MsgTypes>& msg) override {}

	void BroadcastWorld();

	void OnSyncWorldState();

	std::shared_ptr<PlayerCharacter> OnPlayerDataUpdate(int id = 0);

	void SendResurrectPlayer(std::shared_ptr<PlayerCharacter> pl);

	void SendEndInvulnerablePlayer(std::shared_ptr<PlayerCharacter> pl);

};

