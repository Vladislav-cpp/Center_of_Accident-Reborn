#include "SpawnSystem.h"
#include "CharacterFactory.h"
#include "Config.h"
#include "StaticStatsDust.h"
#include "World.h"

#include <cmath>
#include <limits>

void SpawnSystem::Update() {
	for (auto& point : m_xSpawnPoints) {

		if( point.m_iSpawned >= point.m_iMaxBots || !point.m_xSpawnTimer.IsFinished() ) continue;

		int botsToSpawn = std::min(point.m_iBotsPerSpawn, point.m_iMaxBots - point.m_iSpawned);

		for (int i = 0; i < botsToSpawn; ++i) SpawnBotAt(point);

		point.m_xSpawnTimer.Restart();
	}
}

void SpawnSystem::SpawnBotAt(SpawnPoint& point) {
	++point.m_iSpawned;

	auto bot = CharacterFactory::Instance().CreateAIPlayer<StaticStatsDust>();
	bot->SetCoord(point.m_vPos);
	bot->SetID( ++specialID );

	if( const auto& nearestPlayer = FindNearestEnemy(bot->Coord()) ) bot->SetEnemyPlayer(nearestPlayer);
}

std::shared_ptr<PlayerCharacter> SpawnSystem::FindNearestEnemy(const sf::Vector2f& pos) {

	std::shared_ptr<PlayerCharacter> nearest = nullptr;
	float minDistSq = std::numeric_limits<float>::max();

	for( const auto& player : WHumanPlayers() ) {

		if( player->IsInvulnerable() ) continue;
		auto diff = player->Coord() - pos;
		float distSq = diff.x * diff.x + diff.y * diff.y;

		if( distSq < minDistSq ) {
			minDistSq = distSq;
			nearest = player;
		}
	}

	return nearest;
}
