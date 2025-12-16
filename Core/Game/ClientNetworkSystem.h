#pragma once
#include <string>
#include <unordered_set>

#include "olc_net.h"
#include "NetworkCommon.h"

class ClientNetworkSystem : public net::client_interface<MsgTypes> {
	public:
	void Update();

	public:
	int GetMyID() { return m_iMyID; };

	private:
	void OnWorldUpdate();

	private:
	PlayerDescription myDescription;
	uint32_t m_iMyID = 0;
	
	WorldState m_sWorldState;
	std::unordered_set<int> m_existingPlayerIDs;
};