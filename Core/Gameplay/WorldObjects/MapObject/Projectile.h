#pragma once
#include "AICharacter.h"
#include "utility.h"
#include <SFML/Graphics.hpp>
#include <numbers>

class Projectile : public AICharacter {
	public:
	Projectile(sf::Vector2f startCoord, sf::Vector2f targetCoord) : m_vStartCoord(startCoord), m_vTargetCoord(targetCoord) {
		m_vCoord = m_vStartCoord;
		auto [st_x, st_y] = m_vStartCoord;
		auto [tg_x, tg_y] = m_vTargetCoord;

		//float deltaX = tg_x - st_x;
		//float deltaY = tg_y - st_y;

		float deltaX = tg_x - st_x;
		float deltaY = tg_y - st_y;

		std::tie(m_vDirection.x, m_vDirection.y) = utility::normalize(deltaX, deltaY);
		// if(m_vDirection.x==0 && m_vDirection.y==0) __debugbreak();

		m_fAngle = atan2(deltaY, deltaX) * 180.0f / std::numbers::pi;
	}

	virtual void AddUI(UIElement* uiElement) override {
		AICharacter::AddUI(uiElement);
		SetRotate(m_fAngle);
	};

	void SetRotate(float angle) {
		//GetUI()->GetSprite().rotate( sf::degrees(angle) );
		GetUI()->GetSprite().setRotation( sf::degrees(angle) );
	}

	void Move(sf::Vector2f offset) override {
		AICharacter::Move(offset);

		if( m_vCoord.x < 0 || m_vCoord.y < 0 ||  m_vCoord.x > 10000 || m_vCoord.y > 10000) { OnHit(); }
	}

	public:
	void OnHit() { SetHP(0); }

	int GetOwnerID() { return m_iOwnerID; }
	float GetRotate() { return m_fAngle; }
	void SetOwnerID(int id) { m_iOwnerID = id; }

	public:
	sf::Vector2f m_vStartCoord;
	sf::Vector2f m_vTargetCoord;
	sf::Vector2f m_vDirection;

	public:
	float m_fAngle = 0;
	int m_iOwnerID = -1;
};

