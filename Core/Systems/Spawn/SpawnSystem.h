#pragma once

#include "GameTime.h"
#include <SFML/System/Vector2.hpp>

class PlayerCharacter;

struct SpawnPoint {
	sf::Vector2f m_vPos;

	int m_iMaxBots = 0;
	int m_iSpawned = 0;
	int m_iBotsPerSpawn = 1;

	GameTimer m_xSpawnTimer;
};

class SpawnSystem {
	public:
	void Update();

	void AddSpawnPoint(const SpawnPoint& point) {
		m_xSpawnPoints.push_back(point);
	}

	private:
	void SpawnBotAt(SpawnPoint& point);
	std::shared_ptr<PlayerCharacter> FindNearestEnemy(const sf::Vector2f& pos);

	private:
	std::vector<SpawnPoint> m_xSpawnPoints;
	int specialID = 0;
};
