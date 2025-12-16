#pragma once

#include <vector>
#include <memory>
#include "Command.h"

class StaticObject;

class IPlayerController {
	public:
	virtual ~IPlayerController() = default;
	IPlayerController() = default;

	public:
	virtual void SetPlayer(std::shared_ptr<StaticObject> Object);

	public:
	virtual std::vector<std::unique_ptr<Command>> GenerateCommands(float dt) = 0;

	protected:
	std::shared_ptr<StaticObject> m_pObject;
};
