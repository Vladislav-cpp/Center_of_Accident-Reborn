#pragma once

#include "StaticObject.h"
#include "HPLabel.h"

class DestructibleObject : public StaticObject {
	using HpCallback = std::function<void(int)>;

	public:
	virtual ~DestructibleObject() = default;
	DestructibleObject() = default;

	public:
	int HP();
	virtual void SetHP(int hp);
	virtual void ApplyDamage(int damage);

	public:
	void SubscribeOnHpChange(HpCallback cb);

	protected:
	void OnHpChange();

	protected:
	//#define m_iHP m_pDynamicStats->m_iHP
	int m_iMaxHP = 100;

	private:
	std::vector<HpCallback> m_vHpListeners;
};

