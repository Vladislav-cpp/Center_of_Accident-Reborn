#include "Game.h"
#include "ActionHandler.h"
#include "Command.h"
#include "InputState.h"
#include "CharacterFactory.h"
#include "HumenPlayerController.h"
#include "AIPlayerController.h"
#include "Projectile.h"
#include "DestructibleObject.h"
#include "cute_c2.h"
#include <memory>

#include "Config.h"

#include "Client.h"
#include "Server.h"

#include "StaticStatsUfo.h"
#include "StaticStatsDust.h"

Game::Game() : shootSound(shotBuffer), explosion(explosionBuffer), Sprite_Tbackground(Tbackground),
				cursor(sf::Cursor::Type::Arrow) {
	//std::srand(time(NULL));

	Tbackground.loadFromFile("Resources/images/background.png");            Tbackground.setSmooth(true);
	Sprite_Tbackground.setTexture(Tbackground);
	Sprite_Tbackground.setTextureRect( sf::IntRect( {0, 0},{CFG().mapWidth , CFG().mapHeight } ) );
	Sprite_Tbackground.setPosition({0, 0});

	shotBuffer.loadFromFile("Resources/sound and music/shot.ogg");
	explosionBuffer.loadFromFile("Resources/sound and music/explosion.ogg");
	music.openFromFile("Resources/sound and music/background music.ogg");

	window.setFramerateLimit(CFG().fps);
	auto fullscreenMode = sf::VideoMode::getFullscreenModes().front();
	window = sf::RenderWindow{fullscreenMode, "My Game", sf::State::Fullscreen };

	window.setMouseCursor(cursor);

	view = sf::View(
		/* center */ sf::Vector2f{ fullscreenMode.size.x * 0.5f, fullscreenMode.size.y * 0.5f },
		/* size   */ sf::Vector2f{ float(fullscreenMode.size.x), float(fullscreenMode.size.y) }
	);

	// робимо щоб view малювався на весь екран (нормалізовані координати 0..1)
	view.setViewport( sf::FloatRect{{0.f, 0.f}, {1.f, 1.f}} );

	CFG().windowWidth = fullscreenMode.size.x;
	CFG().windowHeight = fullscreenMode.size.y;

	//cursor.createFromSystem( sf::Cursor::Type::Arrow );//написати прицел вместо курсора 
	action = &ActionHandler::Instance();
}

std::int32_t Game::GetTime() {
	return actualTime;
}

std::int32_t Game::GetRechargeTime() {
	return clock_recharge.getElapsedTime().asMilliseconds();
}

sf::Vector2f Game::GetMousePosition() {
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	return window.mapPixelToCoords(pixelPos);
}

std::shared_ptr<PlayerCharacter> Game::GetPlayer(){

	if(mode == GameMode::server) return randServerPlayer;

	return player;
}

void Game::Run() {
}

void Game::ClockRestart() {
	clock.restart();
//	clock_recharge.restart();
//	clock_create_opponents.restart();
}
void Game::ClockRechargeRestart() {
	shootSound.play();
	clock_recharge.restart();
}

void Game::RunServer() {
	mode = GameMode::server;
	
	Server server(60000);
	m_pServer = &server;

	server.Start();
	
	uint32_t UniqueBOT_ID = 0;

	while(true) {

		actualTime =			clock.getElapsedTime().asMilliseconds();
		ClockRestart();

		server.Update(-1, true);
		// fill server data from clients

		/*for(auto ob : server.m_sWorldState.PlayerRst) {
			auto id = ob.first;
		
			auto it = std::find_if(m_vAllObjects.begin(), m_vAllObjects.end(),
				[id](const std::shared_ptr<StaticObject>& obj)
				{
					return obj->ID() == id;
				});

			if(it != m_vAllObjects.end()) {
				it->get()->SetCoord( ob.second.m_vCoord );
				//std::cout<< "set Coord " << ob.second.m_vCoord.x << " - " << ob.second.m_vCoord.y << std::endl;
				//it->get()->SetHP( ob.second.HP );
			}

		}*/

		for (auto& [id, st] : server.m_sWorldState.PlayerRst)
		{
			auto it = std::find_if(m_vAllPlayers.begin(), m_vAllPlayers.end(),
				[&](const auto& p){ return p->ID() == id; });

			if (it != m_vAllPlayers.end())
			{
				(*it)->SetCoord(st.m_vCoord);
			}
		}

		if( !server.m_sWorldState.PlayerRst.empty() && !m_vAllPlayers.empty() ) randServerPlayer = m_vAllPlayers[0];


		// world update
		spawn_bots(++UniqueBOT_ID);


		for(auto& ai : m_vAiPlayers) for( auto& comand : ai->GetController()->GenerateCommands(actualTime) ) comand->Execute();
		for(auto& ai : m_vProjectiles) for( auto& comand : ai->GetController()->GenerateCommands(actualTime) ) comand->Execute();

		ServerCollision();

		//видалення обєктів які знищєні та підрахунок очків класа Iw
		auto IsZeroHP = [](const auto& pl){ return pl->HP()==0;};
		m_vAiPlayers.kill_if(IsZeroHP);
		if( !m_vProjectiles.empty() ) {

		int i = 10+ 10;
		int b = 10 +i;
		int c = i + b + 10;
		}
		
		m_vProjectiles.kill_if(IsZeroHP);

		m_vAllObjects.kill_if([](const auto& ob){ 
			auto dOb = std::dynamic_pointer_cast<DestructibleObject>(ob);
			return dOb != nullptr && dOb->HP()==0;} 
		); 

		//  Sync
		SyncWorldState();

		// fill net data from server
		server.BroadcastWorld();

		// drawWorld();
	}
}

void Game::SyncWorldState() {
	if(m_pServer==nullptr) return;
    // ---- 1. Синхронізація ботів / гравців ----
	auto& world = m_pServer->m_sWorldState;
    //world.BotsRst.clear();

    // -----------------------------
    // 1. Видалити мертві BotsRst ID
    // -----------------------------
    {
        // Зібрати всі поточні ID
        std::unordered_set<uint32_t> aliveBots;
        for (auto& bot : m_vAiPlayers)
        {
            if (bot) aliveBots.insert(bot->ID());
        }

        // Видалити з мапи те, чого немає у aliveBots
        for (auto it = world.BotsRst.begin(); it != world.BotsRst.end(); )
        {
            if (!aliveBots.contains(it->first))
                it = world.BotsRst.erase(it);
            else
                ++it;
        }
    }

    // ------------------------------------------
    // 2. Видалити мертві Projectiles ID
    // ------------------------------------------
    {
        std::unordered_set<uint32_t> aliveProjectiles;
        for (auto& proj : m_vProjectiles)
        {
            if (proj) aliveProjectiles.insert(proj->ID());
        }

        for (auto it = world.Projectiles.begin(); it != world.Projectiles.end(); )
        {
            if (!aliveProjectiles.contains(it->first))
                it = world.Projectiles.erase(it);
            else
                ++it;
        }
    }
    // ------------------------------------------
    // 3. Заново заповнити BotsRst
    // ------------------------------------------
    for (auto& bot : m_vAiPlayers)
    {
        if (!bot) continue;

        PlayerDescription desc;
        desc.ID      = bot->ID();
        desc.HP      = bot->HP();
        desc.m_vCoord = bot->Coord();

        world.BotsRst.insert_or_assign(desc.ID, desc);
    }

    // ------------------------------------------
    // 4. Заново заповнити Projectiles
    // ------------------------------------------
    //world.Projectiles.clear();

    for (auto& proj : m_vProjectiles)
    {
        if (!proj) continue;

        ProjectileDescription desc;
        desc.ID       = proj->ID();
        desc.OwnerID  = proj->GetOwnerID();
        //desc.MousePos = proj->GetMousePos();
        //desc.PlayerPos = proj->GetOwnerPlayerPos();
        desc.m_vCoord = proj->Coord();
        desc.Angle = proj->GetRotate();
		//std::cout << desc.Angle << std::endl;
		//std::cout << desc.ID << std::endl;

        world.Projectiles.insert_or_assign(desc.ID, desc);
    }

    // ------------------------------------------
    // 3. Заново заповнити BotsRst
    // ------------------------------------------
    for (auto& pl : m_vAllPlayers)
    {
        if (!pl) continue;

        PlayerDescription desc;
        desc.ID      = pl->ID();
        desc.HP      = pl->HP();
        desc.m_vCoord = pl->Coord();

        world.PlayerRst.insert_or_assign(desc.ID, desc);
    }


}

//template<typename Pool, typename Map>
//void kill_if_not_in_map(Pool& pool, const Map& mp, int ignorID = -1)
//{
//    pool.kill_if([&](const auto& obj)
//    {
//        uint32_t id = obj->ID();
//
//        // Не видаляти цей ID
//        if(id == ignorID)
//            return false;
//
//        // Якщо ID немає в мапі → видаляємо
//        return mp.find(id) == mp.end();
//    });
//}
//
//template<typename Vec, typename Map, typename Factory>
//void create_missing(Vec& vec, const Map& mp, Factory factory, int ignorID = -1)
//{
//    for (auto& [id, state] : mp)
//    {
//        // Пропускаємо ID, який не треба створювати
//        if (id == ignorID)
//            continue;
//
//        auto it = std::find_if(vec.begin(), vec.end(),
//            [&](const auto& o){ return o->ID() == id; });
//
//        if (it == vec.end())
//        {
//            factory(id, state);
//        }
//    }
//}
//
//void kill_if_not_in_worldstate(utility::ObjectPool<std::shared_ptr<StaticObject>>& pool,
//					const WorldState& ws, int ignorID = -1){
//	pool.kill_if([&](const auto& obj){
//		uint32_t id = obj->ID();
//
//		// Локальний гравець ніколи не видаляємо
//		if(id == ignorID) return false;
//
//		// Якщо ID є в будь-якій з трьох мап → залишаємо
//		if(ws.PlayerRst.find(id) != ws.PlayerRst.end()) return false;
//		if(ws.BotsRst.find(id) != ws.BotsRst.end()) return false;
//		if(ws.Projectiles.find(id) != ws.Projectiles.end()) return false;
//
//		// Інакше → видаляємо
//		return true;
//	});
//}

void Game::ClientSynchronization() {
	//if(player==nullptr) return;

	// === 1. Видаляємо об’єкти, яких нема в мапах ===
	kill_if_not_in_worldstate(m_vAllObjects, m_pClient->m_sWorldState, player->ID());
												
	kill_if_not_in_map(m_vAiPlayers,   m_pClient->m_sWorldState.BotsRst);
	kill_if_not_in_map(m_vProjectiles, m_pClient->m_sWorldState.Projectiles);


	// === 2. Оновлюємо координати існуючих ===

	// Гравці
	for(auto& [id, st] : m_pClient->m_sWorldState.PlayerRst)
	{
		

		auto it = std::find_if(m_vAllPlayers.begin(), m_vAllPlayers.end(),
			[&](const auto& o){ return o->ID() == id; });

		if( it != m_vAllPlayers.end() ) {

			(*it)->SetHP(st.HP);

			if(player->ID() == id) continue;
			(*it)->SetCoord(st.m_vCoord);
		}
	}

	// Боти
	for (auto& [id, st] : m_pClient->m_sWorldState.BotsRst)
	{
		auto it = std::find_if(m_vAiPlayers.begin(), m_vAiPlayers.end(),
			[&](const auto& o){ return o->ID() == id; });

		if (it != m_vAiPlayers.end())
		{
			(*it)->SetCoord(st.m_vCoord);
			(*it)->SetHP(st.HP);
		}
	}

	// пулі
	for (auto& [id, st] : m_pClient->m_sWorldState.Projectiles)
	{
		auto it = std::find_if(m_vProjectiles.begin(), m_vProjectiles.end(),
			[&](const auto& o){ return o->ID() == id; });

		if (it != m_vProjectiles.end())
		{
			(*it)->SetCoord(st.m_vCoord);
			(*it)->SetRotate(st.Angle);
		}
	}


	// === 3. Створюємо ті, яких не було ===
	create_missing(
		m_vAiPlayers,
		m_pClient->m_sWorldState.BotsRst,
		[&](int id, const auto& st){
			auto obj = CharacterFactory::Instance().CreateAIPlayer<StaticStatsDust>();
			obj->SetCoord(st.m_vCoord);
			obj->SetHP(st.HP);
			obj->SetID(st.ID);
			return obj;
		}
	);

	create_missing(
		m_vAllPlayers,
		m_pClient->m_sWorldState.PlayerRst,
		[&](int id, const auto& st){
			auto obj = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>(-1);
			obj->SetCoord(st.m_vCoord);
			obj->SetID(st.ID);
			obj->SetHP(st.HP);
			return obj;
		},
		player->ID()
	);

	create_missing(
		m_vProjectiles,
		m_pClient->m_sWorldState.Projectiles,
		[&](int id, const auto& st){
			auto obj = CharacterFactory::Instance().CreateAIPlayer<StaticStatsProjectile, Projectile>(-1, sf::Vector2f{0, 0}, sf::Vector2f{0, 0});
			obj->SetCoord(st.m_vCoord);
			obj->SetID(st.ID);
			obj->SetOwnerID(st.OwnerID);
			obj->SetRotate(st.Angle);
			return obj;
		}
	);

}

int testTempPj = 50;
void Game::RunClient() {
	// get net info
	//
	mode = GameMode::client;
	Client cl{};
	m_pClient = &cl;
	auto bconect = cl.Connect("185.30.202.137", 60000);
	//auto bconect = cl.Connect("127.0.0.1", 60000);

	std::cout << "bconect - " << bconect << std::endl;

	//std::this_thread::sleep_for(std::chrono::seconds(3));

//	player = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>();
//	player->SetCoord( {pos_spawn_ufo_X, pos_spawn_ufo_Y} );

	while(1) {

		actualTime =			clock.getElapsedTime().asMilliseconds();
		ClockRestart();

		cl.OnClientUpdate(100);
		
		if(player==nullptr) continue;
		if(Iw==nullptr) Iw = new MapHUD(player);

		ClientSynchronization();

		// команди ігрока
		auto commands = player->GetController()->GenerateCommands(actualTime);

		for(auto& cmd : commands) {
			auto rawFireCommand = dynamic_cast<FireCommand*>(cmd.get());

			if( auto raw = dynamic_cast<MovementCommand*>(cmd.get()) ) {
				cmd->Execute();
			} else if( rawFireCommand && testTempPj >0 ) {
				if(GetRechargeTime() <= 250 ) continue;
				--testTempPj;

				ProjectileDescription desc;
				desc.OwnerID = player->ID();
				desc.MousePos = rawFireCommand->GetMousePosition();  // координати мишки
				desc.PlayerPos = player->Coord();        // координати гравця

				net::message<MsgTypes> msg;
				msg.header.id = MsgTypes::Game_AddProjectile;
				msg << desc;
				
				ClockRechargeRestart();
				cl.Send(msg); // відправка серверу

			}

		}


		PlayerDescription outdata;
		outdata.ID = cl.nPlayerID;
		outdata.m_vCoord = player->Coord();
		//outdata.HP = player->HP();

		net::message<MsgTypes> msg;
		msg.header.id = MsgTypes::Game_UpdatePlayer;
		msg << outdata;
		cl.Send(msg);

		//рухаєм камеру
		window.setView(view);

		drawWorld();
	}

}


void Game::RunSingle() {

	actualTime =			clock.getElapsedTime().asMilliseconds();
	auto time =				GetTime();
	float recharge_time =	GetRechargeTime();

	//відкриваєио меню
	Menu_overview();

	ClockRestart();

	//music.play();
	//music.setLooping(true);


	player = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>();
	player->SetCoord( sf::Vector2f(CFG().pos_spawn_ufo_X, CFG().pos_spawn_ufo_Y) );




while( window.isOpen() ) {

	actualTime =			clock.getElapsedTime().asMilliseconds();
	//tempTime =				GetTime();
	float recharge_time =	GetRechargeTime();

	clock.restart();

//	Iw.SetPlayer(uf);

	//включаєм музику якщо та виключилась
	//if(sf::SoundStream::Status::Paused == music.getStatus()) music.play();

	//створення обєктив класа Dust по кд в рандомних позиціях по окружності 
	spawn_bots();
	
	updataCommands();

	//рухаєм вікно	
	float tmp_viev_X = player->Coord().x;
	float tmp_viev_Y = player->Coord().y;

	if( player->Coord().x < (CFG().windowWidth / 2) )			tmp_viev_X = CFG().windowWidth / 2;
	if( player->Coord().x > (3000 - CFG().windowWidth / 2) )		tmp_viev_X = (3000 - CFG().windowWidth / 2);
	if( player->Coord().y < (CFG().windowHeight / 2) )			tmp_viev_Y = CFG().windowHeight / 2;
	if( player->Coord().y > (2000 - CFG().windowHeight / 2) )	tmp_viev_Y = (2000 - CFG().windowHeight / 2);

	Set_coord_viev(tmp_viev_X, tmp_viev_Y);

	// перевірка колізії 
	collision();

	//видалення обєктів які знищєні та підрахунок очків класа Iw
	auto IsZeroHP = [](const auto& pl){ return pl->HP()==0;};
	m_vAiPlayers.kill_if(IsZeroHP);
	m_vProjectiles.kill_if(IsZeroHP);

	m_vAllObjects.kill_if([](const auto& ob){ 
		auto dOb = std::dynamic_pointer_cast<DestructibleObject>(ob);
		return dOb != nullptr && dOb->HP()==0;} 
	); 
	

	//рухаєм камеру
	window.setView(view);

	drawWorld();
}

}

void Game::Menu_overview() {
	sf::Texture TMenu;
	TMenu.loadFromFile("Resources/images/menu/game__menu.png");
	TMenu.setSmooth(true);

	sf::Sprite Smenu(TMenu);
	Smenu.setTextureRect(sf::IntRect( {0, 0}, {1000, 563} ));
	Smenu.setPosition({0, 0});

	sf::Texture TMenu_option;
	TMenu_option.loadFromFile("Resources/images/menu/option.png");
	TMenu_option.setSmooth(true);

	sf::Sprite Smenu_option(TMenu_option);
	Smenu_option.setTextureRect(sf::IntRect({0, 0}, {227, 152}));
	Smenu_option.setPosition({427, 130});

	sf::Texture TMenu_start;
	TMenu_start.loadFromFile("Resources/images/menu/start.png");
	TMenu_start.setSmooth(true);

	sf::Sprite SMenu_start(TMenu_start);
	SMenu_start.setTextureRect( sf::IntRect({0, 0}, {227, 152} ));
	SMenu_start.setPosition( {427, 130} );

	sf::Music music_for_the_menu;
	music_for_the_menu.openFromFile("Resources/sound and music/music for the menu.ogg");
	music_for_the_menu.play();
	music_for_the_menu.setLooping(true);

	bool start = false;
	bool option = false;
	bool menu_is_open = true;

	while(menu_is_open) {

		start = false;
		option = false;

		if(sf::IntRect({435, 146}, {191, 49}).contains(sf::Mouse::getPosition(window))) { start = true; }
		if(sf::IntRect({451, 216}, {159, 49}).contains(sf::Mouse::getPosition(window))) { option = true; }

		if( sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) 
			&& sf::IntRect( {435, 146}, {191, 49}).contains(sf::Mouse::getPosition(window) )) menu_is_open = false;
		
		//закриття вікна і виход з циклу
		while( const std::optional event = window.pollEvent() ) if( event->is<sf::Event::Closed>() ) window.close();


		window.clear();
		window.draw(Smenu);
		if (start)  window.draw(SMenu_start);
		if (option) window.draw(Smenu_option);
		window.display();
	}
}


//створення обєктив класа Dust в рандомних позиціях по окружності
int i = 0;
void Game::spawn_bots(uint32_t id) {
	if(clock_create_opponents.getElapsedTime().asMilliseconds() <=  CFG().createOpponents) return;

	if(++i>=10) return;
	std::cout << "spawn_bots\n"; 

	int tmp_angle_R = rand() % 360;
	CFG().respawn_dust_x = CFG().windowWidth /2 + CFG().radius_spawn_dust_X * cos(tmp_angle_R *  CFG().radians_to_degrees);
	CFG().respawn_dust_y = CFG().windowHeight/2 + CFG().radius_spawn_dust_Y * sin(tmp_angle_R *  CFG().radians_to_degrees);

	const auto& enemy = CharacterFactory::Instance().CreateAIPlayer<StaticStatsDust>();
	enemy->SetID(id);
	enemy->SetCoord( sf::Vector2f(CFG().respawn_dust_x, CFG().respawn_dust_y) );

	clock_create_opponents.restart();
}

void Game::updataCommands() {

	//auto playerComands = std::move( player->GetController()->GenerateCommands() );

	//for(const auto& comand : playerComands) comand->Execute();

	//for(auto& ai : m_vAiPlayers) for( auto& comand : ai->GetController()->GenerateCommands() ) comand->Execute();
	//for(auto& ai : m_vProjectiles) for( auto& comand : ai->GetController()->GenerateCommands() ) comand->Execute();
}

void Game::ServerCollision()
{
    // ---- 1. Побудувати колізії всіх гравців ----
    struct PlayerCollision
    {
        std::shared_ptr<PlayerCharacter> player;
        c2Circle circle;
    };
    std::vector<PlayerCollision> allPlayers;

    for (auto& p : m_vAllPlayers)
    {
        if (!p) continue;
        sf::Sprite& spr = p->GetUI()->GetSprite();
        c2Circle col = utility::GetCircle(spr);
        allPlayers.push_back({ p, col });
    }

    // ---- 2. Побудувати колізії всіх ворогів ----
    struct EnemyCollision
    {
        std::shared_ptr<AICharacter> enemy;
        c2Circle circle;
    };
    std::vector<EnemyCollision> allEnemies;

    for (auto& e : m_vAiPlayers)
    {
        if (!e) continue;
        sf::Sprite& spr = e->GetUI()->GetSprite();
        c2Circle col = utility::GetCircle(spr);
        allEnemies.push_back({ e, col });
    }

    // ---- 3. Player ↔ Player collision ----
    for (size_t i = 0; i < allPlayers.size(); i++)
    {
        for (size_t j = i + 1; j < allPlayers.size(); j++)
        {
            auto& A = allPlayers[i];
            auto& B = allPlayers[j];

            if (c2Collided(&A.circle, nullptr, C2_TYPE_CIRCLE,
                           &B.circle, nullptr, C2_TYPE_CIRCLE))
            {
                A.player->ApplyDamage(10);
                B.player->ApplyDamage(10);
            }
        }
    }

    // ---- 4. Player ↔ Enemy collision ----
    for (auto& pc : allPlayers)
    {
        for (auto& ec : allEnemies)
        {
            if (c2Collided(&pc.circle, nullptr, C2_TYPE_CIRCLE,
                           &ec.circle, nullptr, C2_TYPE_CIRCLE))
            {
                pc.player->ApplyDamage(10);
                // Можна також обробляти пошкодження ворога, якщо потрібно:
                // ec.enemy->ApplyDamage(10);
            }
        }
    }

    // ---- 5. Projectile → Player collision ----
    for (auto& proj : m_vProjectiles)
    {
        sf::Sprite& spr = proj->GetUI()->GetSprite();
        c2Capsule projCapsule = utility::GetCapsule(spr);

        // Player
        for (auto& pc : allPlayers)
        {
            if( proj->GetOwnerID() == pc.player->ID() ) continue;

            if (c2Collided(&projCapsule, nullptr, C2_TYPE_CAPSULE,
                           &pc.circle, nullptr, C2_TYPE_CIRCLE))
            {
                pc.player->ApplyDamage(10);
                proj->OnHit();
            }
        }

        // Enemy
        for (auto& ec : allEnemies)
        {
            if (proj->GetOwnerID() == ec.enemy->ID()) continue;

            if (c2Collided(&projCapsule, nullptr, C2_TYPE_CAPSULE,
                           &ec.circle, nullptr, C2_TYPE_CIRCLE))
            {
                // Шкода ворогу
                ec.enemy->ApplyDamage(10);
                proj->OnHit();
            }
        }
    }

    // ---- 6. Enemy ↔ Enemy collision (якщо потрібно) ----
    //for (size_t i = 0; i < allEnemies.size(); i++)
    //{
    //    for (size_t j = i + 1; j < allEnemies.size(); j++)
    //    {
    //        auto& A = allEnemies[i];
    //        auto& B = allEnemies[j];

    //        if (c2Collided(&A.circle, nullptr, C2_TYPE_CIRCLE,
    //                       &B.circle, nullptr, C2_TYPE_CIRCLE))
    //        {
    //            // Обробка зіткнення ворогів, якщо потрібно
    //            // A.enemy->ApplyDamage(5);
    //            // B.enemy->ApplyDamage(5);
    //        }
    //    }
    //}
}



void Game::collision() {
	auto playerColision = utility::GetCircle( player->GetUI()->GetSprite() );
	
	// ---- Enemy collisions ----
	for(const auto& enemy : m_vAiPlayers) {
		auto enemyCollision = utility::GetAABB( enemy->GetUI()->GetSprite() );

		// Collision check
		bool hit = c2Collided( &playerColision, NULL, C2_TYPE_CIRCLE, &enemyCollision, NULL, C2_TYPE_AABB );
		
		if( hit ) enemy->ApplyDamage(100);
	}

	for(auto& proj : m_vProjectiles) {
		auto projCapsule = utility::GetCapsule( proj->GetUI()->GetSprite() );
		
		for(auto& enemy : m_vAiPlayers) {
			auto enemyCollision = utility::GetAABB( enemy->GetUI()->GetSprite() );
			
			int hit = c2Collided( &projCapsule, NULL, C2_TYPE_CAPSULE, &enemyCollision, NULL, C2_TYPE_AABB );
			
			if(hit) {
				enemy->ApplyDamage( 10 );
				proj->OnHit();
			}
		}
	}

}

void Game::drawWorld() {

	view.setCenter( utility::ClampToMap(player->Coord(), CFG().windowWidth, CFG().windowHeight, CFG().mapWidth, CFG().mapHeight) );
	window.setView(view);

	//очистка вікна
	window.clear();

	window.draw(Sprite_Tbackground);

	//закриття вікна і виход з циклу
	while( const std::optional event = window.pollEvent() ) if( event->is<sf::Event::Closed>() ) window.close();

	//аніміровання та отрисовка всіх обєктів 
	for(const auto& ob : m_vAllObjects) {
		ob->Draw(window, actualTime);

		utility::DrawDebug( window, ob->GetUI()->GetSprite(), ob->GetStaticStats() );
	}

	//аніміровання та отрисовка інформаційного вікнa
	if( Iw!=nullptr ) Iw->Draw(window, actualTime);

	action->Clean();

	window.display();
}

void Game::createMainPlayer(int id) {
	player = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>();
	player->SetID(id);
	player->SetCoord(sf::Vector2f(CFG().pos_spawn_ufo_X, CFG().pos_spawn_ufo_Y) );
}
void Game::createPlayer(int idController, int id) {
	auto pl = CharacterFactory::Instance().CreateHumanPlayer<StaticStatsUfo>(idController);
	pl->SetID(id);
}

void Game::RegisterObject(std::shared_ptr<StaticObject> ob) {

	m_vAllObjects.push_back(ob);

	if(		 auto pt = std::dynamic_pointer_cast<Projectile>(ob) )	m_vProjectiles.push_back(pt);
	else if( auto ai = std::dynamic_pointer_cast<AICharacter>(ob) )	m_vAiPlayers.push_back(ai);
	else if( auto pl = std::dynamic_pointer_cast<PlayerCharacter>(ob) )	m_vAllPlayers.push_back(pl);
}



