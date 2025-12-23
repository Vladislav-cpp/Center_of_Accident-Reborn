#pragma once
#include <string>
#include <unordered_set>

#include "net_all.h"
#include "NetworkCommon.h"

class GameSession;

class ClientNetworkSystem : public net::tcp_client<MsgTypes>, public net::udp_client<MsgTypes> {
	public:
	bool Connect(const std::string& host, const uint16_t port);

	void Update();
	void Update_TCP();
	void Update_UDP();
	public:
	int GetMyID() { return m_iMyID; };

	public:
	void SendPlayerState(const GameSession& seddion);
	void SendCommand(Command* comm);

	private:
	void OnWorldUpdate();

	private:
	PlayerDescription myDescription;
	uint32_t m_iMyID = 0;
	
	WorldState m_sWorldState;
	std::unordered_set<int> m_existingPlayerIDs;
};