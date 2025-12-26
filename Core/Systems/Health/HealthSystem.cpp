#include "HealthSystem.h"
#include "DestructibleObject.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include "World.h"
#include "GameTime.h"
#include "Config.h"
#include "UIType.h"
#include "ServerNetworkSystem.h"

const int ImmunityDuration = 5; // second

void HealthSystem::Update(ServerNetworkSystem* network) {
    ProcessDestroy();

    ProcessResurrectPlayer(network);
}

void HealthSystem::ProcessDestroy() {
	auto IsZeroHP = [](const auto& ob){ return ob->HP() == 0; };

	WAIPlayers().kill_if(IsZeroHP);
	WProjectiles().kill_if(IsZeroHP);

	WAllObjects().kill_if([](const auto& ob){ 
		auto dOb = std::dynamic_pointer_cast<DestructibleObject>(ob);
		auto pl = std::dynamic_pointer_cast<PlayerCharacter>(ob);

		return pl == nullptr && dOb != nullptr && dOb->HP() == 0;
	});
}

void HealthSystem::ProcessResurrectPlayer(ServerNetworkSystem* network) {
	for( auto& pl :  WHumanPlayers() ) {

        int id = pl->ID();

		if( pl->HP() == 0 ) {

			pl->SetCoord( FindSafeSpawnPoint() );
            pl->SetHP(100);

			GameTimer t(ImmunityDuration);

			m_vImmunityDurations[id] = t;

            pl->ChangeUIState(UIType::Shield, UIState::Visible);

            network->SendResurrectPlayer(pl);

		} else if( auto playerIt = m_vImmunityDurations.find(id); playerIt != m_vImmunityDurations.end() && playerIt->second.IsFinished() ) {

            m_vImmunityDurations.erase(id);

            pl->ChangeUIState(UIType::Shield, UIState::Hidden);
            network->SendEndInvulnerablePlayer(pl);
		}
	}
}

bool HealthSystem::IsAreaClear(sf::Vector2f pos, float radius) {
	for( auto& obj : WAllObjects() ) {
		sf::Vector2f objPos = obj->Coord();
		float distance = std::sqrt(std::pow(pos.x - objPos.x, 2) + std::pow(pos.y - objPos.y, 2));

		if(distance < radius) {
			return false;
		}
	}
	return true;
}

sf::Vector2f HealthSystem::FindSafeSpawnPoint() {
    float w = static_cast<float>(CFG().mapWidth);
    float h = static_cast<float>(CFG().mapHeight);
    float margin = 50.0f; // Відступ від самого краю карти

    // Координати 4 кутів
    std::vector<sf::Vector2f> corners = {
        {margin, margin},             // Топ-ліво
        {w - margin, margin},         // Топ-право
        {margin, h - margin},         // Бот-ліво
        {w - margin, h - margin}      // Бот-право
    };

    // Перевіряємо кожен кут
    for(const auto& corner : corners) {
        if(IsAreaClear(corner, 100.0f)) {
            return corner;
        }
    }

    // Якщо кути зайняті — шукаємо випадкове місце (макс. 50 спроб, щоб не зависнути)
    for(int i = 0; i < 50; ++i) {
        sf::Vector2f randomPos;
        randomPos.x = margin + static_cast<float>(rand() % static_cast<int>(w - 2 * margin));
        randomPos.y = margin + static_cast<float>(rand() % static_cast<int>(h - 2 * margin));

        if(IsAreaClear(randomPos, 100.0f)) {
            return randomPos;
        }
    }

    // Крайній випадок: якщо все забито, повертаємо центр (або будь-який кут)
    return {w / 2.0f, h / 2.0f};
}



