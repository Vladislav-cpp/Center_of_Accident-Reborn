#pragma once
#include "MovableObject.h"
#include "AIPlayerController.h"

class AICharacter : public MovableObject {
	public:
	virtual ~AICharacter() {};
	AICharacter() {};

	public:
	void SetEnemyPlayer(const std::shared_ptr<PlayerCharacter>& pl) { 
		AIPlayerController* aiCtrl = static_cast<AIPlayerController*>(m_pController.get());
		aiCtrl->SetEnemyPlayer(pl);
	}
};