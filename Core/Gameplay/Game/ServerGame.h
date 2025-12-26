#pragma once

class ServerNetworkSystem;
class SpawnSystem;

class ServerGame {
	public:
	ServerGame();
	~ServerGame();

	public:
	void Run();

	private:
	ServerNetworkSystem* m_xNetwork;
	SpawnSystem* m_xSpawner;
};