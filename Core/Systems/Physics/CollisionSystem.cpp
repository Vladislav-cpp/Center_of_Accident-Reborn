#include "CollisionSystem.h"
#include "PlayerCharacter.h"
#include "AICharacter.h"
#include "Projectile.h"
#include "World.h"
#include "cute_c2.h"

void CollisionSystem::UpdateCollisions() {
    // ---- 1. Побудувати колізії всіх гравців ----
    struct PlayerCollision
    {
        std::shared_ptr<PlayerCharacter> player;
        c2Circle circle;
    };
    std::vector<PlayerCollision> allPlayers;

    for(auto& p : WHumanPlayers()) {
        sf::Sprite& spr = p->GetUI()->GetSprite();
        c2Circle col = utility::GetCircle(spr);
        allPlayers.push_back({ p, col });
    }

    // ---- 2. Побудувати колізії всіх ворогів ----
    struct EnemyCollision
    {
        std::shared_ptr<AICharacter> enemy;
        c2AABB aabb;
    };
    std::vector<EnemyCollision> allEnemies;

    for (auto& e : WAIPlayers()) {
        sf::Sprite& spr = e->GetUI()->GetSprite();
        c2AABB col = utility::GetAABB(spr);
        allEnemies.push_back({ e, col });
    }

    // ---- 3. Player ↔ Player collision ----
    for(size_t i = 0; i < allPlayers.size(); i++) {
        for(size_t j = i + 1; j < allPlayers.size(); j++) {
            auto& A = allPlayers[i];
            auto& B = allPlayers[j];

            if( c2Collided(&A.circle, nullptr, C2_TYPE_CIRCLE,
                           &B.circle, nullptr, C2_TYPE_CIRCLE)) {
                A.player->ApplyDamage(10);
                B.player->ApplyDamage(10);
            }
        }
    }

    // ---- 4. Player ↔ Enemy collision ----
    for(auto& pc : allPlayers) {
        for(auto& ec : allEnemies) {
            if( c2Collided(&pc.circle, nullptr, C2_TYPE_CIRCLE,
                           &ec.aabb, nullptr, C2_TYPE_AABB) ) {
                pc.player->ApplyDamage(10);
                // Можна також обробляти пошкодження ворога, якщо потрібно:
                // ec.enemy->ApplyDamage(10);
            }
        }
    }

    // ---- 5. Projectile → Player collision ----
    for (auto& proj : WProjectiles()) {
        sf::Sprite& spr = proj->GetUI()->GetSprite();
        c2Capsule projCapsule = utility::GetCapsule(spr);

        // Player
        for(auto& pc : allPlayers) {
            if( proj->GetOwnerID() == pc.player->ID() ) continue;

            if( c2Collided(&projCapsule, nullptr, C2_TYPE_CAPSULE,
                           &pc.circle  , nullptr, C2_TYPE_CIRCLE)) {
                pc.player->ApplyDamage(10);
                proj->OnHit();
            }
        }

        // Enemy
        for(auto& ec : allEnemies) {
            if(proj->GetOwnerID() == ec.enemy->ID()) continue;

            if( c2Collided(&projCapsule, nullptr, C2_TYPE_CAPSULE,
                           &ec.aabb , nullptr, C2_TYPE_AABB) ) {
                // Шкода ворогу
                ec.enemy->ApplyDamage(10);
                proj->OnHit();
            }
        }
    }

    // ---- 6. Enemy ↔ Enemy collision (якщо потрібно) ----
    //for (size_t i = 0; i < allEnemies.size(); i++)
    //{
    //    for (size_t j = i + 1; j < allEnemies.size(); j++)
    //    {
    //        auto& A = allEnemies[i];
    //        auto& B = allEnemies[j];

    //        if (c2Collided(&A.circle, nullptr, C2_TYPE_CIRCLE,
    //                       &B.circle, nullptr, C2_TYPE_CIRCLE))
    //        {
    //            // Обробка зіткнення ворогів, якщо потрібно
    //            // A.enemy->ApplyDamage(5);
    //            // B.enemy->ApplyDamage(5);
    //        }
    //    }
    //}

}