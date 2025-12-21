#include "StaticObject.h"
#include "UIElement.h"
#include "IStats.h"

int StaticObject::ID() {
	return m_iID;
}

sf::Vector2f StaticObject::Coord() {
	return m_vCoord;
}

void StaticObject::SetCoord(sf::Vector2f coord) {
	m_vCoord = coord;
	OnCoordChange();
}

void StaticObject::SetID(int id) {
	m_iID = id;
}

void StaticObject::AddUI(UIElement* uiElement) {
	m_vUIViews.push_back( uiElement );

	uiElement->Update();
}

void StaticObject::SetController(std::unique_ptr<IPlayerController> controller) {
	controller->SetPlayer( this->shared_from_this() );
	m_pController = std::move( controller );
}


void StaticObject::UpdateViewCoord() {
	for(const auto& view : m_vUIViews) view->SetCoord(m_vCoord);
}

const void StaticObject::Draw(sf::RenderWindow& window, float time) {
	for(const auto& view : m_vUIViews) view->Draw(window, time);
}

void StaticObject::OnCoordChange() {
	for(const auto& fn : m_vCoordListeners) fn(m_vCoord);

	UpdateViewCoord();
}

void StaticObject::SubscribeOnCoordChange(CoordCallback cb) {
	m_vCoordListeners.push_back(cb);
}


