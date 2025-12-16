#pragma once
#include <memory>
#include <string>

#include "IStats.h"

class StaticStatsDust : public IStaticStatsBase {
public:
    StaticStatsDust () {
        objectName = "Dust";
        UI_Protorype = "Dust_1";
        m_vBehaviorState = { std::make_unique<FollowPlayerState>() };

        speedMovement = 150;
    };
};
