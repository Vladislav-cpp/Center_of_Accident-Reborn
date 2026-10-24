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
	virtual std::vector<std::unique_ptr<Command>> GenerateCommands(const InputState& state);
	virtual std::vector<std::unique_ptr<Command>> GenerateCommands(float dt) { return std::vector<std::unique_ptr<Command>>();};
	virtual void SetPlayer(std::shared_ptr<StaticObject> Object) override;

	private:
	std::unique_ptr<MovementCommand> CreateMoveCommand(const InputState& state);
	std::unique_ptr<FireCommand> CreateAttackCommand(const InputState& state);

	private:
	GameTimer* m_pAttacktimer;
};

