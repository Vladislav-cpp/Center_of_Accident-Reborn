#pragma once
#include "IPlayerController.h"
#include "MovementCommand.h"
#include "FireCommand.h"

struct InputState;
class GameTimer;

class HumenPlayerController : public IPlayerController {
	public:
	HumenPlayerController();
	virtual ~HumenPlayerController();

	public:
	virtual std::vector<std::unique_ptr<Command>> GenerateCommands(float dt) override;
	virtual void SetPlayer(std::shared_ptr<StaticObject> Object) override;

	private:
	std::unique_ptr<MovementCommand> CreateMoveCommand(float dt);
	std::unique_ptr<FireCommand> CreateAttackCommand();

	private:
	GameTimer* m_pAttacktimer;
};

