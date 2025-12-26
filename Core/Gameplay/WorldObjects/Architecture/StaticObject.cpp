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

void StaticObject::AddUI(UIType type, UIElement* uiElement) {
	m_vUIViews[type] = uiElement;

	uiElement->Update();
}

void StaticObject::ChangeUIState(UIState newState) {
	for(auto const& [type, view] : m_vUIViews) view->ChangeState(newState);
}

void StaticObject::ChangeUIState(UIType type, UIState newState) {
	if(auto& ui = m_vUIViews.at(type)) ui->ChangeState(newState);
	else std::cout << "UIType missing" << std::endl;
}

void StaticObject::SetController(std::unique_ptr<IPlayerController> controller) {
	controller->SetPlayer( this->shared_from_this() );
	m_pController = std::move( controller );
}


void StaticObject::UpdateViewCoord() {
	for(auto const& [type, view] : m_vUIViews) view->SetCoord(m_vCoord);
}

UIElement* StaticObject::GetUI(UIType type){
	auto it = m_vUIViews.find(type);
	return (it != m_vUIViews.end()) ? it->second : nullptr;
}

const void StaticObject::Draw(sf::RenderWindow& window, float time) {
	for(auto const& [type, view] : m_vUIViews) view->Draw(window, time);
}

void StaticObject::OnCoordChange() {
	for(const auto& fn : m_vCoordListeners) fn(m_vCoord);

	UpdateViewCoord();
}

void StaticObject::SubscribeOnCoordChange(CoordCallback cb) {
	m_vCoordListeners.push_back(cb);
}


