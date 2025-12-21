#include "World.h"
#include "ServerNetworkSystem.h"
#include "Projectile.h"
#include "AICharacter.h"
#include "PlayerCharacter.h"

void World::RegisterObject(std::shared_ptr<StaticObject> ob) {

	allObjects.push_back(ob);

	if(		 auto pt = std::dynamic_pointer_cast<Projectile>(ob) )	projectiles.push_back(pt);
	else if( auto ai = std::dynamic_pointer_cast<AICharacter>(ob) )	aiPlayers.push_back(ai);
	else if( auto pl = std::dynamic_pointer_cast<PlayerCharacter>(ob) )	humanPlayers.push_back(pl);
}