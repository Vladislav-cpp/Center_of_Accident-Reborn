#include "Render.h"
#include "World.h"
#include "StaticObject.h"
#include "GameSession.h"



void Render::DrawWorld( float time, bool drawDebug) {

//	view.setCenter( utility::ClampToMap(player->Coord(), CFG().windowWidth, CFG().windowHeight, CFG().mapWidth, CFG().mapHeight) );
	window.setView(m_xSession.view);

	//очистка вікна
	//window.clear();

	//закриття вікна і виход з циклу
	//while( const std::optional event = window.pollEvent() ) if( event->is<sf::Event::Closed>() ) window.close();

	//аніміровання та отрисовка всіх обєктів 
	for(const auto& ob : WAllObjects()) {
		ob->Draw(window, time);

		//utility::DrawDebug( window, ob->GetUI()->GetSprite(), ob->GetStaticStats() );
	}

	//window.display();


}