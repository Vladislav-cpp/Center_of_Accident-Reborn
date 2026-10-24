#pragma once
#include "MovableObject.h"
#include "HumenPlayerController.h"

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
		auto newState = bInvulnerable ? UIState::Visible : UIState::Hidden;
		ChangeUIState(UIType::Shield, newState);
	}

	bool IsInvulnerable() {
		return GetUI(UIType::Shield)->GetState() == UIState::Visible;
	}

	virtual void ApplyDamage(int damage) override {
		if( !IsInvulnerable() ) MovableObject::ApplyDamage(damage);
	}

	// TODO біда з цим контролером, тому що  GenerateCommands(const InputState& state); не можна звести один для аі і гравця
	HumenPlayerController& GetHumenController() { return static_cast<HumenPlayerController&>(*m_pController); } 
};
