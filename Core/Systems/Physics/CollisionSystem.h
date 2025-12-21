#pragma once
#include <ImplSingleton.h>

class CollisionSystem : public ImplSingleton<CollisionSystem> {
	friend class ImplSingleton<CollisionSystem>;

	public:
	void UpdateCollisions();

};

inline CollisionSystem& CollSys() { return CollisionSystem::Instance(); }