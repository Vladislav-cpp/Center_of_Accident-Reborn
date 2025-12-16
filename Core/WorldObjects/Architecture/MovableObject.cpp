#include "MovableObject.h"

void MovableObject::Move(sf::Vector2f offset) {
	SetCoord(m_vCoord + offset);
}
