#pragma once
#include <memory>
#include <string>

#include "IStats.h"

class StaticStatsUfo : public IStaticStatsBase {
public:
    StaticStatsUfo () {
        objectName = "Ufo";
        UI_Protorype = "Ufo_1";
        speedMovement = 210;
        rechargeTime = 0.3;
    };
};
