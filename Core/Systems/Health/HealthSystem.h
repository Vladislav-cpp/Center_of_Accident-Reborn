#pragma once
#include <map>
#include <SFML/System/Vector2.hpp>
#include <ImplSingleton.h>

class GameTimer;
class ServerNetworkSystem;

class HealthSystem : public ImplSingleton<HealthSystem> {
	friend class ImplSingleton<HealthSystem>;
    public:
    HealthSystem() = default;
    ~HealthSystem() = default;

    void Update(ServerNetworkSystem* network);

    private:
    void ProcessDestroy();
    void ProcessResurrectPlayer(ServerNetworkSystem* network);
    sf::Vector2f FindSafeSpawnPoint();
    bool IsAreaClear(sf::Vector2f pos, float radius);

    private:
    std::map<int, GameTimer> m_vImmunityDurations;
};

inline HealthSystem& HealthSys() { return HealthSystem::Instance(); }