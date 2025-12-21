#pragma once
#include "DestructibleObject.h"

class MovableObject : public DestructibleObject {
	public:
	virtual ~MovableObject() = default;
	MovableObject() = default;

	public:
	virtual void Move(sf::Vector2f offset);

	private:
	float m_fMoveSpeed = 0;

};

