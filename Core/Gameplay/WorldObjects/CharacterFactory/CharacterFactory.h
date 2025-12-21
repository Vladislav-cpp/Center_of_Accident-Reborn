#pragma once

#include "ImplSingleton.h"
#include "IStats.h"
#include "PlayerCharacter.h"
#include "AICharacter.h"
#include "AIPlayerController.h"
#include "HumenPlayerController.h"
#include "UIElement.h"
#include "Projectile.h"
#include "ReflectionLib.hpp"
#include <memory>
#include <utility>

class CharacterFactory : public ImplSingleton<CharacterFactory> {
	friend class ImplSingleton<CharacterFactory>;
	public:
	
	template<typename ClassStats>
	std::shared_ptr<PlayerCharacter> CreateHumanPlayer(int idController = 0) {
		auto state = ClassStats();
		return DoCreateHumanPlayer( state, idController );
	}

	std::shared_ptr<PlayerCharacter> DoCreateHumanPlayer(IStaticStatsBase& stats, int idController);


	template<typename ClassStats, typename AICharacterClass = AICharacter, typename... Args>
	std::shared_ptr<AICharacterClass> CreateAIPlayer( int idController = 0, Args&&... args ) {
		auto state = ClassStats();
		auto aiPlayer = std::make_shared<AICharacterClass>(std::forward<Args>(args)...);
		return std::dynamic_pointer_cast<AICharacterClass>( DoCreateAIPlayer(aiPlayer, state, idController) );
	}

	std::shared_ptr<AICharacter> DoCreateAIPlayer(std::shared_ptr<AICharacter> aiPlayer, IStaticStatsBase& stats, int idController);

	void CustomStep(std::shared_ptr<StaticObject> ob, IStaticStatsBase& stats);

};

