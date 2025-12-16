#pragma once
#include <memory>
#include <string>

#include "IStats.h"

class StaticStatsProjectile : public IStaticStatsBase {
public:
    StaticStatsProjectile () {
        objectName = "Chuck";
        m_vBehaviorState = { std::make_unique<ProjectileBehaviorState>() };

        UI_Protorype = "Chuck_1";
        speedMovement = 450;
        haveHPbar = false;
    };
};
