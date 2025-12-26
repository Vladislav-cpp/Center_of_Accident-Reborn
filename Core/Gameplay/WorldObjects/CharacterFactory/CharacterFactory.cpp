#include "CharacterFactory.h"
#include "HPLabel.h"
#include <memory>
#include <World.h>


void CharacterFactory::CustomStep(std::shared_ptr<StaticObject> object, IStaticStatsBase& stats) {
	IDynamicStatsBase dynamicStats{};
	object->SetStats( stats, dynamicStats);

	auto objects = reflection::Ref.m_xUI_Objects;

	auto it = std::find_if(objects.begin(), objects.end(),
		[&](const auto& p) { return stats.UI_Protorype.has_value() && p.second == stats.UI_Protorype; });

	if( it == objects.end() ) {
		std::cerr << "Reflection error: object is not a UIElement!\n";
		return;
	}

	if( UIElement* uiElem = reinterpret_cast<UIElement*>( it->first ) ) object->AddUI( uiElem->GetType(), uiElem->Clone() );
	else std::cerr << "Reflection error: object is not a UIElement!\n";

	W().RegisterObject(object);

	if( !stats.haveHPbar ) return;
	sf::Sprite& sprite = object->GetUI()->GetSprite();
	
	auto bounds = sprite.getLocalBounds(); // Rect<float>
	sf::Vector2f scale = sprite.getScale();
	
	sf::Vector2f size = {
		bounds.size.x * scale.x,	// width
		bounds.size.y * scale.y		// height
	};
	HPLabel* hpLable = new HPLabel();	

	hpLable->InitForObject(object->Coord(), size, dynamicStats.m_iHP);
	object->AddUI( hpLable->GetType(), hpLable );

	if( auto dOb = std::dynamic_pointer_cast<DestructibleObject>(object) ) dOb->SubscribeOnHpChange( [hpLable](int hp){ hpLable->SetHP(hp); } );
	else std::cerr << "Reflection error: dynamic_pointer_cast - not DestructibleObject!\n";
}

std::shared_ptr<PlayerCharacter> CharacterFactory::DoCreateHumanPlayer(IStaticStatsBase& stats, int idController) {
	auto player = std::make_shared<PlayerCharacter>();
	//player->name generate unique name

	CustomStep(player, stats);

	if(idController == -1) return player;
	// if or switch idController 
	auto controller = std::make_unique<HumenPlayerController>();
	player->SetController( std::move(controller) ); // ACHTUNG controller deleted after

	return player;
}

std::shared_ptr<AICharacter> CharacterFactory::DoCreateAIPlayer(std::shared_ptr<AICharacter> aiPlayer, IStaticStatsBase& stats, int idController) {
	//auto aiPlayer = std::make_shared<AICharacter>();
	//player->name generate unique name

	CustomStep(aiPlayer, stats);

	if(idController == -1) return aiPlayer;
	// if or switch idController 
	auto controller = std::make_unique<AIPlayerController>();
	controller->AddBehavior( stats.m_vBehaviorState );
	aiPlayer->SetController( std::move(controller) ); // ACHTUNG controller deleted after

	return aiPlayer;
}
