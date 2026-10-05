#pragma once
#include <string>
#include <unordered_set>

#include "net_all.h"
#include "NetworkCommon.h"

class GameSession;
class PlayerCharacter;

class ClientNetworkSystem : public net::tcp_client<MsgTypes>, public net::udp_client<MsgTypes> {
	public:
	using ConnectionStateHandler = std::function<void(net::ConnectionState oldState, net::ConnectionState newState)>;

	public:
	bool Connect(const std::string& host, const uint16_t port);

	void Update();
	void Update_TCP();
	void Update_UDP();
	public:
	int GetMyID() { return m_iMyID; };

	public:
	void SetSession(GameSession* session);

	void SetOnConnectionStateChanged(ConnectionStateHandler handler) {
		m_onConnectionStateChanged = std::move(handler);
	}

	public:
	void SendJoinRequest();

	public:
	void SendPlayerState();
	void SendCommand(Command* comm);

	void UpdateInterpolation(float dt);

	private:
	void InterpolateEntities(const WorldState& stateA, const WorldState& stateB, float t);
	void WorldUpdate(const WorldState& worldState);

	private:
	PlayerDescription myDescription;
	uint32_t m_iMyID = 0;
	GameSession * m_pSession;
	
	private:
	std::deque<WorldSnapshot> m_snapshotBuffer;
	const size_t MAX_BUFFER_SIZE = 30;

	private:
	std::unordered_set<int> m_existingPlayerIDs;

	private:
	ConnectionStateHandler m_onConnectionStateChanged;
	net::ConnectionState m_lastKnownTcpState = net::ConnectionState::Idle;

	private:
	float m_fCurrentInterpolationTime = 0.0f; 
	const float m_fInterpolationDelay = 0.1f; // Затримка 100 мс (6 тіків при 60Гц)
};