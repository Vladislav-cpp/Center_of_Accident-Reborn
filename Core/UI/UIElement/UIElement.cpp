#include "UIElement.h"
#include "ReflectionLib.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>


REFLECT_TYPE( 
	UIElement,
	{},
	FIELDS_WRAPPER(	REFLECT_FIELD(UIElement, m_fAnimationSpeed),
			//		REFLECT_FIELD(UIElement, m_xTexture),
			//        REFLECT_FIELD(UIElement, m_xSprite),
					REFLECT_FIELD(UIElement, m_vPosition),
					REFLECT_FIELD(UIElement, m_sTextureFile),
					REFLECT_FIELD(UIElement, m_vProportions),
					REFLECT_FIELD(UIElement, m_iDistanceAnimation),
					REFLECT_FIELD(UIElement, m_iNumberFrames),
					REFLECT_FIELD(UIElement, m_vSpriteOrigin),
					REFLECT_FIELD(UIElement, m_vSpriteScale),
					REFLECT_FIELD(UIElement, m_vFrameOffset),
					REFLECT_FIELD(UIElement, m_xType),
					REFLECT_FIELD(UIElement, m_fCurentFrame) ),

	//METHODS_WRAPPER(   /*REFLECT_METHOD(UIElement, Get_Sprite),*/
	//				REFLECT_METHOD(UIElement, SetData),
	//				REFLECT_METHOD(UIElement, Animation),
	//				REFLECT_METHOD(UIElement, Draw) )
{}
);

void UIElement::SetData(std::string fileName, sf::Vector2i frameOffset, sf::Vector2i proportions, int distanceAnimation, int numberFrames) {
	auto bLoad = m_xTexture.loadFromFile(fileName);
	if( !bLoad ) std::cerr << "wrong load file - " << fileName << std::endl;

	m_xSprite.setTexture(m_xTexture);

	m_vFrameOffset = frameOffset;
	m_vProportions = proportions;

	m_xSprite.setTextureRect(sf::IntRect( m_vFrameOffset , m_vProportions) );
	m_iDistanceAnimation = distanceAnimation;
	m_iNumberFrames = numberFrames;
	m_fCurentFrame = 0;
}

void UIElement::Animation(float time) {
	if( m_iNumberFrames==0 ) return;
	m_fCurentFrame += m_fAnimationSpeed* time;

	if( m_fCurentFrame >= m_iNumberFrames ) m_fCurentFrame = 0;
	int newX = m_vFrameOffset.x + static_cast<int>(m_fCurentFrame)*m_iDistanceAnimation;

	m_xSprite.setTextureRect( sf::IntRect( {newX, (int)m_vFrameOffset.y}, m_vProportions) );
}


void UIElement::Draw(sf::RenderWindow& app, float time) {
	if( m_xState != UIState::Visible ) return;

	if( m_iNumberFrames!=0 ) Animation(time);
	app.draw(m_xSprite);
}

void UIElement::Update() {
	if( !m_xTexture.loadFromFile(m_sTextureFile) ) throw std::runtime_error("Failed to load texture from file: " + m_sTextureFile);

	m_xSprite.setTexture(m_xTexture, true);
	m_xSprite.setTextureRect(sf::IntRect(m_vFrameOffset, m_vProportions));
	m_xSprite.setOrigin(m_vSpriteOrigin);
	if(m_vSpriteScale.x != 0 && m_vSpriteScale.y != 0 ) m_xSprite.setScale(m_vSpriteScale);
}

void UIElement::SetCoord(sf::Vector2f position) {
	m_vPosition = position;
	m_xSprite.setPosition(m_vPosition);
}

void UIElement::ChangeState(UIState newState) {
	m_xState = newState;
}
