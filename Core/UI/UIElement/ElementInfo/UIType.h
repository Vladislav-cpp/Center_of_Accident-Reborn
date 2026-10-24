#pragma once

#include <string>
#include <stdexcept>

// for buttost
// NameScreen (- #Screen) + name button (- #m_u) 
// example MainMenuScreen have m_uStar  -->  MainMenuStart

#define UI_TYPE_LIST(X)     \
    X( Screen )             \
    X( Main )               \
    X( Unknown )            \
    X( HealthLabel )        \
    X( Shield )             \
    X( ConnectionState )    \
    X( MainMenuStart )      \
    X( MainMenuOption )     



enum class UIType {
#define AS_ENUM(name) name,
    UI_TYPE_LIST(AS_ENUM)
#undef AS_ENUM
};

#define SCREEN_SUB_TYPE_LIST(X) \
    X( None )                   \
    X( MainMenu )               \
    X( GameScreen )             \
    X( GlobalOverlay )          


enum class ScreenSubType {
#define AS_ENUM(name) name,
    SCREEN_SUB_TYPE_LIST(AS_ENUM)
#undef AS_ENUM
};

inline UIType UITypeFromString(const std::string& str) {
#define AS_IF_ELSE_UIType(name) if (str == #name) return UIType::name;
    UI_TYPE_LIST(AS_IF_ELSE_UIType)
#undef AS_IF_ELSE

    throw std::invalid_argument("Unknown UIType: " + str); // кращє логувати
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

    throw std::invalid_argument("Unknown UI State: " + str);
}

