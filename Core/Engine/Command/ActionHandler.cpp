#include "ActionHandler.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

void ActionHandler::SetWindow(sf::RenderWindow* window) {
    m_xWindow = window;
}

void ActionHandler::ActionsImput(const sf::Event& event) {

    if(const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        switch(key->code) {
            case sf::Keyboard::Key::W: m_bWPressed = true; break;
            case sf::Keyboard::Key::S: m_bSPressed = true; break;
            case sf::Keyboard::Key::A: m_bAPressed = true; break;
            case sf::Keyboard::Key::D: m_bDPressed = true; break;
            default: break;
        }
    }

    if(const auto* key = event.getIf<sf::Event::KeyReleased>()) {
        switch(key->code) {
            case sf::Keyboard::Key::W: m_bWPressed = false; break;
            case sf::Keyboard::Key::S: m_bSPressed = false; break;
            case sf::Keyboard::Key::A: m_bAPressed = false; break;
            case sf::Keyboard::Key::D: m_bDPressed = false; break;
            default: break;
        }
    }

    if( const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>() ) {

        if(mouse->button == sf::Mouse::Button::Left) {
            m_bLeftMousePressed = true;
        }
    }

    if( const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>() ) {

        if( mouse->button == sf::Mouse::Button::Left ) {
            m_bLeftMousePressed = false;
        }
    }

    // Якщо вікно втратило фокус — скидаємо стани.
    // можна це не завжди робити , можна винести в налаштування
    if( event.is<sf::Event::FocusLost>() ) {
        m_bWPressed = false;
        m_bSPressed = false;
        m_bAPressed = false;
        m_bDPressed = false;
        m_bLeftMousePressed = false;
    }
}

InputState ActionHandler::Update() {

    if( !m_xWindow || !m_xWindow->hasFocus() ) {
        m_bWPressed = false;
        m_bSPressed = false;
        m_bAPressed = false;
        m_bDPressed = false;
        m_bLeftMousePressed = false;
    } else {
        // Перевіряємо фактичний стан клавіш щокадру.
        m_bWPressed = sf::Keyboard::isKeyPressed( sf::Keyboard::Key::W );
        m_bSPressed = sf::Keyboard::isKeyPressed( sf::Keyboard::Key::S );
        m_bAPressed = sf::Keyboard::isKeyPressed( sf::Keyboard::Key::A );
        m_bDPressed = sf::Keyboard::isKeyPressed( sf::Keyboard::Key::D );

        // Перевіряємо фактичний стан кнопки миші.
        m_bLeftMousePressed = sf::Mouse::isButtonPressed( sf::Mouse::Button::Left );

        // Оновлюємо позицію курсора.
        m_xState.mousePosition = m_xWindow->mapPixelToCoords( sf::Mouse::getPosition(*m_xWindow) );
    }

    // Розраховуємо напрямок руху.
    sf::Vector2f moveDir(0.f, 0.f);

    if(m_bWPressed) moveDir.y -= 1.f;
    if(m_bSPressed) moveDir.y += 1.f;
    if(m_bAPressed) moveDir.x -= 1.f;
    if(m_bDPressed) moveDir.x += 1.f;

    m_xState.moveOffset = moveDir;
    m_xState.bMove = (moveDir.x != 0.f || moveDir.y != 0.f);

    m_xState.bFire = m_bLeftMousePressed;

    return m_xState;
}