#include "FireCommand.h"
#include "Game.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include "CharacterFactory.h"
#include "ActionHandler.h"
#include <SFML/System/Vector2.hpp>
#include "StaticStatsProjectile.h"

void FireCommand::Execute() {
	auto player = dynamic_pointer_cast<PlayerCharacter>(m_pObject);
	
	auto action = &ActionHandler::Instance();
	CharacterFactory::Instance().CreateAIPlayer<StaticStatsProjectile, Projectile>(0, player->Coord(), ActionHandler::Instance().ActionsImput().mousePosition);
}

void FireCommand::SetMousePosition(sf::Vector2f position) {
	m_vMousePosition = position;
}

sf::Vector2f FireCommand::GetMousePosition() {
	return m_vMousePosition;
}
