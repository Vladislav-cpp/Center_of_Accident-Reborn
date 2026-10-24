#pragma once

class UIElement;

struct ButtonPressed {
	UIType m_xType = UIType::Unknown;
};

// --- Model -> UI ---

class PlayerCharacter;

struct HUDInitData { 
	const std::shared_ptr<PlayerCharacter> player;
};

