#include "HPLabel.h"

HPLabel::HPLabel() {
    // фон
    m_BackgroundBar.setFillColor(sf::Color(50, 50, 50, 200));
    // HP бар
    m_ForegroundBar.setFillColor(sf::Color(0, 220, 0, 255));

    m_xType = UIType::HealthLabel;
}

void HPLabel::InitForObject(sf::Vector2f objPos, sf::Vector2f objSize, int maxHp) {
    m_iMaxHp = maxHp;
    m_ObjectSize = objSize;

    float barWidth = objSize.x * 0.8f;  // ширина 80% ширини об’єкта
    float barHeight = objSize.y * 0.12f; // висота 12% висоти

    m_BackgroundBar.setSize({barWidth, barHeight});
    m_ForegroundBar.setSize({barWidth, barHeight});

    SetCoord(objPos);
}

void HPLabel::SetHP(int hp) {
    if(m_iMaxHp <= 0) m_iMaxHp = 1;
    m_HPPercent = std::max(0.f, std::min(1.f, hp / float(m_iMaxHp)));

    // оновлюємо ширину зеленої полоски
    float fullWidth = m_BackgroundBar.getSize().x;
    m_ForegroundBar.setSize({fullWidth * m_HPPercent, m_BackgroundBar.getSize().y});
}

void HPLabel::SetCoord(sf::Vector2f position) {
    const float offset = 10.0f; // наскільки підняти HP-бар над об’єктом

    // Беремо розміри об'єкта з m_ObjectSize
    float objWidth  = m_ObjectSize.x;
    float objHeight = m_ObjectSize.y;

    // Обчислюємо лівий верхній кут HP-бару
    float barX = position.x - objWidth / 2.0f + (objWidth - m_BackgroundBar.getSize().x) / 2.0f;
    float barY = position.y - objHeight / 2.0f - m_BackgroundBar.getSize().y - offset;

    m_BackgroundBar.setPosition({barX, barY});
    m_ForegroundBar.setPosition({barX, barY});

    // Якщо треба, оновлюємо UIElement
 //   UIElement::SetCoord(position);
}


void HPLabel::Draw(sf::RenderWindow& app, float time) {
    // Якщо у лейбла є своя графіка UIElement, то малюємо її
    // if(m_iNumberFrames != 0) Animation(time);
//    app.draw(m_xSprite);

    // Малюємо HP
    app.draw(m_BackgroundBar);
    app.draw(m_ForegroundBar);
}
