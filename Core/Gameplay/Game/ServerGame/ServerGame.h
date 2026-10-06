#pragma once

class ServerNetworkSystem;
class AIManager;

class ServerGame {
	public:
	ServerGame();
	~ServerGame();

	public:
	void Run();

	private:
	ServerNetworkSystem* m_xNetwork;
	AIManager* m_xAIManager;
};