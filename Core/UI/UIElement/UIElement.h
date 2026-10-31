#pragma once
#include <SFML/Graphics.hpp>
#include "ReflectionLib.hpp"
#include "UIType.h"

class UIElement {
	REFLECTION_FRIEND;

	public:
	UIElement() : m_xSprite(m_xTexture) {};
	virtual ~UIElement() {};

	virtual UIElement* Clone() const {
		return new UIElement(*this);
	}

	public:
	void CreateSimpleSquare(sf::Vector2u size, sf::Color color);
	virtual void SetData(std::string fileName, sf::Vector2i frameOffset, sf::Vector2i proportions, int distanceAnimation, int numberFrames);
	virtual void Animation(float time);
	virtual void Draw(sf::RenderWindow& app, float time = 0);

	public:
	virtual void Update();
	virtual void SetCoord(sf::Vector2f position);
	virtual void ChangeState(UIState newState);

	public:
	bool Contains(sf::Vector2f point) const {
		return m_xSprite.getGlobalBounds().contains(point);
	}

	bool Contains(sf::Vector2i point) const {
		return m_xSprite.getGlobalBounds().contains( sf::Vector2f(point) );
	}

	public:
	sf::Sprite& GetSprite() { return m_xSprite; }
	sf::Texture& GetxTexture() { return m_xTexture; }
	UIType GetType() { return m_xType; }
	UIState GetState() { return m_xState; }

	protected:
	float m_fAnimationSpeed = 0.02f;
	sf::Texture m_xTexture{};
	sf::Sprite m_xSprite;

	protected:
	sf::Vector2f m_vSpriteScale = {0, 0};
	sf::Vector2f m_vSpriteOrigin = {0, 0};
	sf::Vector2f m_vPosition = {0, 0};
	sf::Vector2i m_vProportions = {1, 1};
	sf::Vector2i m_vFrameOffset = {0, 0};
	int m_iDistanceAnimation = 0;
	int m_iNumberFrames = 0;

	std::string m_sTextureFile;
	float m_fCurentFrame = 0;

	public:
	UIType m_xType = UIType::Unknown;
	UIState m_xState = UIState::Visible;
};

