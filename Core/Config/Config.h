#pragma once
#include <string>
#include "ReflectionLib.hpp"
#include "ImplSingleton.h"

class Config : public ImplSingleton<Config> {
	friend class ImplSingleton<Config>;

	public:
	REFLECTION_FRIEND;
	static constexpr double radians_to_degrees = 57.2958;

	int windowWidth = 1920;
	int windowHeight = 1080;
	int mapWidth  = 3000;
	int mapHeight = 2000;
	std::string windowTitle = "My Game";

	int numberOpponents = 0;
	int recharge = 300;
	int createOpponents = 1000;

	int pos_spawn_ufo_X = 500;
	int pos_spawn_ufo_Y = 281;

	int respawn_dust_x = 0;
	int respawn_dust_y = 0;
	int radius_spawn_dust_X = 450;
	int radius_spawn_dust_Y = 450;

	int fps = 60;

	float gravity = 9.8;
};

inline Config& CFG() { 
	return Config::Instance(); 
}
