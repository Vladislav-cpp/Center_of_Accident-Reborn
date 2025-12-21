#pragma once

#include "IPlayerController.h"
#include "MovementCommand.h"
#include "BehaviorState.h"

class PlayerCharacter;

class AIPlayerController : public IPlayerController {
	public:
	virtual ~AIPlayerController() = default;
	AIPlayerController() = default;

	public:
	virtual std::vector<std::unique_ptr<Command>> GenerateCommands(float dt);

	public:
	void AddBehavior(std::vector<std::shared_ptr<IBehaviorState>>& behaviors);
	void SetEnemyPlayer(const std::shared_ptr<PlayerCharacter>& pl);

	private:
	std::vector<std::shared_ptr<IBehaviorState>> m_vBehaviors;
	std::shared_ptr<PlayerCharacter> player = nullptr;
};

