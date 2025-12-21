#pragma once
#include <memory>
#include <vector>
#include "Command.h"

class AICharacter;
class PlayerCharacter;

class IBehaviorState {
    public:
    virtual ~IBehaviorState() = default;
    virtual std::vector<std::unique_ptr<Command>> GenerateCommands(float dt) { __debugbreak(); return std::vector<std::unique_ptr<Command>>(); };

    std::shared_ptr<AICharacter> ai = nullptr;
    std::shared_ptr<PlayerCharacter> pl = nullptr;
};

#define DEFINE_BEHAVIOR_STATE(ClassName) \
class ClassName : public IBehaviorState { \
public: \
    std::vector<std::unique_ptr<Command>> GenerateCommands(float dt) override; \
};

DEFINE_BEHAVIOR_STATE(FollowPlayerState)
DEFINE_BEHAVIOR_STATE(ProjectileBehaviorState)


