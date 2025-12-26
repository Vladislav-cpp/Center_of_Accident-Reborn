#include "ReflectionLib.hpp"
#include "UIElement.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Config.h"
#include "Utility.hpp"
#include "ActionHandler.h"
#include <SFML/System/Vector2.hpp>
#include "CharacterFactory.h"
#include "StaticStatsUfo.h"
#define CUTE_C2_IMPLEMENTATION
#include "cute_c2.h"
#include "UIType.h"

void register_basic_type_parsers() {
    reflection::Ref.register_type_parser<sf::Vector2i>([](void* ptr, const std::vector<std::string>& v){
        if(v.size() >= 2) *static_cast<sf::Vector2i*>(ptr) = { std::stoi(v[0]), std::stoi(v[1]) };
    });

    reflection::Ref.register_type_parser<sf::Vector2f>([](void* ptr, const std::vector<std::string>& v){
        if(v.size() >= 2) *static_cast<sf::Vector2f*>(ptr) = { std::stof(v[0]), std::stof(v[1]) };
    });

    reflection::Ref.register_type_parser<UIType>([](void* ptr, const std::vector<std::string>& v){
        *static_cast<UIType*>(ptr) = UITypeFromString(v[0]) ;
    });

    reflection::Ref.register_type_parser<UIState>([](void* ptr, const std::vector<std::string>& v){
        *static_cast<UIState*>(ptr) = UIStateFromString(v[0]) ;
    });
}

#include "ClientGame.h"
#include "ServerGame.h"

int main() {

    //c2Circle c1 = { {0, 0}, 10 };
    //c2Circle c2 = { {5, 0}, 10 };

    //int result = c2CircletoCircle(c1, c2); // 1 = перетинаються
    //std::cout << "Collision: " << result << std::endl;

    UIElement::technical_work();
    Config::technical_work();

    reflection::register_type<sf::Vector2f>({}, {}, {});
    reflection::register_type<sf::Vector2i>({}, {}, {});
    reflection::register_type<UIState>({}, {}, {});
    reflection::register_type<UIType>({}, {}, {});
    register_basic_type_parsers();


    auto path = utility::GetExecutablePath();
    std::cout << path << std::endl;
    reflection::Ref.build_objects_from_xml( path + "/Resources/Config/UIConfig.xml");

    //std::cout << reflection::get_type_id<int>()    << " " <<     
    //reflection::get_type_id<float>()               << " " <<
    //reflection::get_type_id<std::string>()         << " " <<
    //reflection::get_type_id<double>()              << " " <<
    //reflection::get_type_id<sf::Vector2i>()        << " " <<
    //reflection::get_type_id<bool>()                << " " <<
    //reflection::get_type_id<sf::Vector2f>()                ;

    reflection::DataLoader loader;
    loader.load(path + "/Resources/Config/config.cfg");

    reflection::Type* cfgType = reflection::get_type<Config>();
    Config* cfg = static_cast<Config*>(reflection::createObjectFromData(cfgType, loader));

    std::cout << "WindowWidth = " << cfg->windowWidth << "\n";
    //auto& Cyber_Game = Game::Instance();

    int id = -1;
    std::cin >> id;
    //if(id == 0) Cyber_Game.RunClient();
    //else if(id == 1103) Cyber_Game.RunSingle();
    //else  Cyber_Game.RunServer();


    if(id == 0) {
        ClientGame* cl = new ClientGame();
        cl->Run();
    } else {
        ServerGame* sr = new ServerGame();
        sr->Run();
    }

    return 0;
}