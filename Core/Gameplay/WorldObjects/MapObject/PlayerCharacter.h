#pragma once
#include "MovableObject.h"

class PlayerCharacter : public MovableObject {
	public:
	virtual ~PlayerCharacter() {};

	PlayerCharacter() {
		auto objects = reflection::Ref.m_xUI_Objects;

		auto shield3 = std::find_if(objects.begin(), objects.end(),
		[&](const auto& p) { return  p.second == "shield3"; });
	
		if( UIElement* uiElem = reinterpret_cast<UIElement*>( shield3->first ) ) {
			uiElem->ChangeState(UIState::Hidden);
			AddUI( uiElem->GetType(), uiElem->Clone() );
		}

	};

	void SetInvulnerable(bool bInvulnerable) {
		auto newState = bInvulnerable ? UIState::Hidden : UIState::Visible;
		ChangeUIState(UIType::Shield, newState);
	}

	bool IsInvulnerable() {
		return GetUI(UIType::Shield)->GetState() == UIState::Hidden;
	}

	virtual void ApplyDamage(int damage) override {
		if( !IsInvulnerable() ) MovableObject::ApplyDamage(damage);
	}
};
