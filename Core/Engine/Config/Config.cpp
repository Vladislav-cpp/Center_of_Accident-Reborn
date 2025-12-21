#include "Config.h"

REFLECT_TYPE(
	Config,
	{},
	FIELDS_WRAPPER(
		REFLECT_FIELD(Config, windowWidth),
		REFLECT_FIELD(Config, windowHeight),
		REFLECT_FIELD(Config, windowTitle),
		REFLECT_FIELD(Config, numberOpponents),
		REFLECT_FIELD(Config, recharge),
		REFLECT_FIELD(Config, createOpponents),
		REFLECT_FIELD(Config, fps),
		REFLECT_FIELD(Config, pos_spawn_ufo_X),
		REFLECT_FIELD(Config, pos_spawn_ufo_Y),
		REFLECT_FIELD(Config, respawn_dust_x),
		REFLECT_FIELD(Config, respawn_dust_y),
		REFLECT_FIELD(Config, radius_spawn_dust_X),
		REFLECT_FIELD(Config, radius_spawn_dust_Y),
		REFLECT_FIELD(Config, gravity)
	),
	{}
);
