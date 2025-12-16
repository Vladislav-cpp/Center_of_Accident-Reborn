#include "BehaviorState.h"
#include "AICharacter.h"
#include "PlayerCharacter.h"
#include "Projectile.h"
#include "MovementCommand.h"
#include "FireCommand.h"
#include "Game.h"
#include "IStats.h"
#include <tuple>

std::vector<std::unique_ptr<Command>> FollowPlayerState::GenerateCommands(float dt){
    std::vector<std::unique_ptr<Command>> commands;

    dt *= ai->GetPrototpe()->speedMovement.value();

    auto [targetX, targetY] = pl->Coord();
//    std::cout << targetX << " " << targetY << "\n";
    auto [oldMyX, oldMyY] = ai->Coord();
    float distance = std::sqrt(std::pow(targetX - oldMyX, 2) + std::pow(targetY - oldMyY, 2));

    if(distance < 5) return commands;

    auto newMyX = oldMyX + (dt * (targetX - oldMyX) / distance);
    auto newMyY = oldMyY + (dt * (targetY - oldMyY) / distance);

    auto command = std::make_unique<MovementCommand>();
    command->SetObject( ai );
    command->SetShift({ newMyX - oldMyX, newMyY - oldMyY });
    commands.push_back(std::move(command));

    return commands;
}

std::vector<std::unique_ptr<Command>> ProjectileBehaviorState::GenerateCommands(float dt) {
    auto projectile = std::dynamic_pointer_cast<Projectile>(ai);

    dt *= ai->GetPrototpe()->speedMovement.value();

    auto [oldPosX, oldPosY] =  projectile->m_vStartCoord;
	
	auto newPosX = oldPosX + dt * projectile->m_vDirection.x;
	auto newPosY = oldPosY + dt * projectile->m_vDirection.y;


    auto command = std::make_unique<MovementCommand>();
    command->SetObject( ai );
    command->SetShift( { newPosX - oldPosX, newPosY - oldPosY } );

    std::vector<std::unique_ptr<Command>> commands;
    commands.push_back(std::move(command));
    return commands;
}
