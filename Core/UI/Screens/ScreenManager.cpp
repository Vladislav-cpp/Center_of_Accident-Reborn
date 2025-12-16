#include "ScreenManager.h"
#include "Screen.h"

void ScreenManager::Push(std::unique_ptr<Screen> screen) {
	m_xStack.push_back(std::move(screen));
}

void ScreenManager::Pop() {
	if( !m_xStack.empty() ) m_xStack.pop_back();
}

Screen* ScreenManager::Current() {
	return m_xStack.empty() ? nullptr : m_xStack.back().get();
}