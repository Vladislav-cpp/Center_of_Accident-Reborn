#include "IPlayerController.h"
#include "StaticObject.h"

void IPlayerController::SetPlayer(std::shared_ptr<StaticObject> object) {
	m_pObject = object;
}