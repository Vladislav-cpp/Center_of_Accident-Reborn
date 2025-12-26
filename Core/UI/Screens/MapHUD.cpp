#include "MapHUD.h"
#include "PlayerCharacter.h"
#include "Config.h"
#include "utility.h"
#include <iostream>
#include <cassert>

const static std::string s_sCoin = "Resources/images/Full Coins.png";
const static std::string s_sSheet = "Resources/images/Sprite Sheet.png";
const static std::string s_sMistral = "Resources/type/Mistral.ttf";
// coin(s_sCoin, 0, 0, 15, 16, 16, 8),
// life_(s_sSheet, 0, 0, 35, 32, 0, 0),
MapHUD::MapHUD(std::shared_ptr<PlayerCharacter> pl)
	: player(pl), textÑoin(font, { "x"+std::to_string( 0 ) }, 35), textLife(font, "x"+std::to_string( 0 ), 35) {
	
	assert( font.openFromFile(s_sMistral) ); // "Font file not found or could not be opened"

	coin = new UIElement();
	coin->SetCoord( {10, 10} );
	coin->GetSprite().setScale({2, 2});
	coin->SetData(s_sCoin, {0, 0}, {15, 16}, 16, 8);

	life = new UIElement();
	life->SetCoord( {8, 45} );
	life->SetData(s_sSheet, {0, 0}, {35, 32}, 0, 0);

	textÑoin.setStyle(sf::Text::Bold);
	textÑoin.setPosition( {50, 2} );
	//text.setFillColor(sf::Color::Black);
	//textÑoin.setPosition( {1050,2} );
	
	textLife.setStyle(sf::Text::Bold);
	textLife.setPosition({50, 37});
	//textLife.setPosition( {1050, 37} );
}

void MapHUD::Update() {
	//textÑoin.setString( "x" + std::to_string(/* pl->Point()*/10 ));
	//textLife.setString( "x" + std::to_string( player->HP() ) );

	sf::Vector2f playerPos = player->Coord();

	// îôñåò HUD
	sf::Vector2f offset(-CFG().windowWidth / 2.f + 10.f, -CFG().windowHeight / 2.f + 10.f);

	// îáìåæóºìî HUD ïî êàðò³
	sf::Vector2f clampedPos = utility::ClampToMap(playerPos, CFG().windowWidth, CFG().windowHeight, 3000.f, 2000.f);

	// ïîçèö³¿ òåêñò³â
	textLife.setPosition( clampedPos + offset + sf::Vector2f(50.f, 37.f) );
	textÑoin.setPosition( clampedPos + offset + sf::Vector2f(50.f, 2.f) );

	// îíîâëåííÿ çíà÷åíü
	textLife.setString( "HP: " + std::to_string( player->HP() ));
	textÑoin.setString( "Coins: " + std::to_string(/*player->GetCoins()*/ 10 ));

	// UI åëåìåíòè
	life->SetCoord( clampedPos + offset + sf::Vector2f(8.f, 45.f)	);
	coin->SetCoord( clampedPos + offset + sf::Vector2f(10.f, 10.f)	);
}

void MapHUD::Draw(sf::RenderWindow& app, int time) {
	Update();

	app.draw(textLife);
	app.draw(textÑoin);

	life->Draw(app);
	coin->Draw(app, time);
}


