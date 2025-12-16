#pragma once
#include "UIElement.h"

class HPLabel : public UIElement {
    public:
    HPLabel();

    HPLabel* Clone() const override {
        return new HPLabel(*this);
    }

    // встановити позицію об’єкта і його розміри
    void InitForObject(sf::Vector2f objPos, sf::Vector2f objSize, int maxHp);

    // встановити HP
    void SetHP(int hp);

    void Update() override {};

    // перемістити при русі об’єкта
    void SetCoord(sf::Vector2f position) override;

    // малювання хп бару
    void Draw(sf::RenderWindow& app, float time = 0) override;

    private:
    sf::RectangleShape m_BackgroundBar; // чорна/сіра підложка
    sf::RectangleShape m_ForegroundBar; // зелений бар HP
    
    sf::Vector2f m_ObjectSize{};
    float m_HPPercent = 1.f;
    int m_iMaxHp = 0;
};
