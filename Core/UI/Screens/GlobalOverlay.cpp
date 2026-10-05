#include "GlobalOverlay.h"
#include "map"

const static std::map<net::ConnectionState, std::string> s_connectionImages = {
	{ net::ConnectionState::Idle, "" },
	{ net::ConnectionState::Connecting, "Resources/Images/ConnectionStatus/connecting.png" },
	{ net::ConnectionState::Connected, "Resources/Images/ConnectionStatus/connecting_succes.png" },
	{ net::ConnectionState::Failed, "Resources/Images/ConnectionStatus/connecting_failed.png" }
};
 
GlobalOverlay::GlobalOverlay(sf::RenderWindow& window, ScreenManager& screens) : Screen(window, screens) {

	auto objects = reflection::Ref.m_xUI_Objects;

	auto it = std::find_if(objects.begin(), objects.end(),
		[&](const auto& p) { return p.second == "ConnectionState"; });

	if( it == objects.end() ) {
		std::cerr << "Reflection error: object is not a UIElement!\n";
		return;
	}

	if( UIElement* uiElem = reinterpret_cast<UIElement*>( it->first ) ) m_uConnectionStatus = uiElem->Clone() ;
	else std::cerr << "Reflection error: object is not a UIElement!\n";

	m_uConnectionStatus->SetCoord({10.f, 10.f});

}

void GlobalOverlay::RefreshStatus(net::ConnectionState state) {

	m_uConnectionStatus->SetData(s_connectionImages.at(state), sf::Vector2i(0, 0), sf::Vector2i(0, 0), 0, 0);
	//m_uConnectionStatus->GetSprite().setTexture( s_connectionImages.at(state) );
}

void GlobalOverlay::Update(float dt) {

	

}

void GlobalOverlay::Draw(float dt) {
	Update(dt);
	
	m_uConnectionStatus->Draw(m_xWindow);
}