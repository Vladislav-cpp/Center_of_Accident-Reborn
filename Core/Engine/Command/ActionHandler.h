#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "ImplSingleton.h"

#include "InputState.h"

class ActionHandler : public ImplSingleton<ActionHandler> {
	friend class ImplSingleton<ActionHandler>;
public:
    void SetWindow(sf::RenderWindow* window);

    void ActionsImput(const sf::Event& event);
    InputState Update();

private:
    sf::RenderWindow* m_xWindow = nullptr;
    InputState m_xState;

    bool m_bWPressed = false;
    bool m_bSPressed = false;
    bool m_bAPressed = false;
    bool m_bDPressed = false;

    bool m_bLeftMousePressed = false;
};