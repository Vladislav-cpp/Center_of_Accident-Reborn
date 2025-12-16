#pragma once
#include "Utility.h"
#include "ImplSingleton.h"

class PlayerCharacter;
class AICharacter;
class StaticObject;
class Projectile;

class World : public ImplSingleton<World>  {
	friend class ImplSingleton<World>;

	public:
	void RegisterObject(std::shared_ptr<StaticObject> ob);

	public:
	utility::ObjectPool<std::shared_ptr<Projectile>> projectiles;
	utility::ObjectPool<std::shared_ptr<AICharacter>> aiPlayers;
	utility::ObjectPool<std::shared_ptr<PlayerCharacter>> humanPlayers;

	utility::ObjectPool<std::shared_ptr<StaticObject>> allObjects;

};

inline World& W() { return World::Instance(); }

inline auto& WProjectiles() { return World::Instance().projectiles; }
inline auto& WAIPlayers() { return World::Instance().aiPlayers; }
inline auto& WHumanPlayers() { return World::Instance().humanPlayers; }

inline auto& WAllObjects() { return World::Instance().allObjects; }