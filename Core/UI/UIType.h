#pragma once

#include <string>
#include <stdexcept>

#define UI_TYPE_LIST(X) \
    X( Main )             \
    X( HealthLabel )      \
    X( Shield )           

enum class UIType {
#define AS_ENUM(name) name,
    UI_TYPE_LIST(AS_ENUM)
#undef AS_ENUM
};

inline UIType UITypeFromString(const std::string& str) {
#define AS_IF_ELSE_UIType(name) if (str == #name) return UIType::name;
    UI_TYPE_LIST(AS_IF_ELSE_UIType)
#undef AS_IF_ELSE

    throw std::invalid_argument("Unknown UIType: " + str);
}


#define UI_STATE_LIST(X) \
    X( None )        \
    X( Visible )     \
    X( Hidden )

enum class UIState {
#define AS_ENUM(name) name,
    UI_STATE_LIST(AS_ENUM)
#undef AS_ENUM
};

inline UIState UIStateFromString(const std::string& str) {
#define AS_IF_ELSE_UIState(name) if (str == #name) return UIState::name;
    UI_STATE_LIST(AS_IF_ELSE_UIState)
#undef AS_IF_ELSE

    throw std::invalid_argument("Unknown UIType: " + str);
}

