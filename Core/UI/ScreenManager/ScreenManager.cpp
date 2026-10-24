#include "ScreenManager.h"
#include "Screen.h"

void ScreenManager::Push(std::unique_ptr<Screen> screen) {
	const ScreenSubType type = screen->GetSubType();
	m_xStack.emplace(type, std::move(screen));
}

bool ScreenManager::HandleEvent(const sf::Event& e) {

	if( auto* screen = Current() ) {
		return screen->HandleEvent(e);
	}

	return false;
}

void ScreenManager::Draw(float dt) {

	for( auto* screen:OpenScreens() ) {
		screen->Draw(dt);
	}
}

Screen* ScreenManager::Current() {

    if (m_xCurrentScreens.empty()) {
        return nullptr;
    }

    auto it = m_xStack.find(m_xCurrentScreens.back());

    if (it == m_xStack.end()) {
        return nullptr;
    }

    return it->second.get();
}

std::vector<Screen*> ScreenManager::OpenScreens() {

    std::vector<Screen*> screens;
    screens.reserve(m_xCurrentScreens.size());

    for(ScreenSubType type : m_xCurrentScreens) {

        auto it = m_xStack.find(type);

        if( it != m_xStack.end() ) {
            screens.push_back( it->second.get() );
        }
    }

    return screens;
}

#include "INI_ScreenManagerExtensions.h"