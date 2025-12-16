#include "DestructibleObject.h"

int DestructibleObject::HP() {
	return m_pDynamicStats->m_iHP;
}

void DestructibleObject::SetHP(int hp) {
	m_pDynamicStats->m_iHP = hp;

	OnHpChange();
}

void DestructibleObject::ApplyDamage(int damage) {
	auto oldHp = HP();
	auto hp = m_pDynamicStats->m_iHP;
	hp -= damage;

	if(hp <= 0) hp = 0;
	if(hp>100) __debugbreak;

	SetHP(hp);
}

void DestructibleObject::SubscribeOnHpChange(HpCallback cb) {
	m_vHpListeners.push_back(cb);
}

void DestructibleObject::OnHpChange() {
	for(const auto& fn :m_vHpListeners) fn(m_pDynamicStats->m_iHP);
}
