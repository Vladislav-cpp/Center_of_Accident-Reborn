#pragma once
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "BehaviorState.h"

template <typename Child>
class IStats {
    public:
    virtual ~IStats() = default;
    IStats() = default;

    std::unique_ptr<Child> clone() const {
        return std::make_unique<Child>(static_cast<const Child&>(*this));
    }
};

class IDynamicStatsBase : public IStats<IDynamicStatsBase> {
    public:
    uint32_t m_iID = 0;
    int32_t m_iHP = 100;
//    sf::Vector2f m_vCoord{0.f, 0.f};
};

//#define CREATE_ALIAS(obj, field) decltype(IDynamicStatsBase::field)& field = obj->field;
//
//#define EXPAND_ALIASES(obj, first, ...) \
//    CREATE_ALIAS(obj, first) \
//    __VA_OPT__(EXPAND_ALIASES(obj, __VA_ARGS__))
//
//#define DYNAMIC_STATS_FIELDS(obj) EXPAND_ALIASES(obj, m_iID, m_vCoord)

class IStaticStatsBase : public IStats<IStaticStatsBase> {
    public:
    std::optional<std::string> UI_Protorype;
    std::optional<std::string> objectName;
    std::optional<float> speedMovement;
    std::optional<float> rechargeTime;
    bool haveHPbar = true;

    std::vector<std::shared_ptr<IBehaviorState>> m_vBehaviorState;
};
