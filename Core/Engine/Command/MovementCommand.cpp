#include "MovementCommand.h"
#include "StaticObject.h"
#include "MovableObject.h"

void MovementCommand::Execute() {
	dynamic_pointer_cast<MovableObject>(m_pObject)->Move(m_fShiftXY);
}

void MovementCommand::SetShift(sf::Vector2f ShiftXY) {
	m_fShiftXY = ShiftXY;
}

void MovementCommand::SetDT(float dt) {
	const float speed = m_pObject->GetPrototpe()->speedMovement.value();
	m_fShiftXY *= speed * dt;
}
