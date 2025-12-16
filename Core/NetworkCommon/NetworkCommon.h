#pragma once

#include "IStats.h"
#include "StaticObject.h"
#include "utility.h"
#include "olc_net.h"

enum class MsgTypes : uint32_t {
	Server_GetStatus,

	Client_Accepted,
	Client_RegisterWithServer,
	Client_AssignID,

//	Game_UpdateEnemy,
	Game_WorldUpdate,

	Game_AddProjectile,

	Game_UpdatePlayer,
	Game_RemovePlayer,
	Game_AddPlayer,
};

struct PlayerDescription {
	public:
	sf::Vector2f m_vCoord{0.f, 0.f};
	
	uint32_t ID = 0;
	uint32_t HP = 0;
//	uint32_t StaticStatsID = 0; // 1 - dust// 2 - projectile // 3 - ufo
//	uint32_t nAvatarID = 0;
};

struct ProjectileDescription {
	uint32_t ID = 0;            // ID, сервер присвоює
	uint32_t OwnerID = 0;       // гравець, який стріляє

	sf::Vector2f MousePos;      // координати мишки на момент стрілу
	sf::Vector2f PlayerPos;     // позиція гравця на момент стрілу
	sf::Vector2f m_vCoord{0.f, 0.f};
	float Angle = 0;
};

struct WorldState {
	std::unordered_map<uint32_t, PlayerDescription> PlayerRst;
	std::unordered_map<uint32_t, PlayerDescription> BotsRst;
	std::unordered_map<uint32_t, ProjectileDescription> Projectiles;
};

// Для unordered_map
template<typename K, typename V>
inline void PackList(net::message<MsgTypes>& msg, const std::unordered_map<K, V>& map) {
    uint32_t count = static_cast<uint32_t>(map.size());
    for (const auto& [key, value] : map) {
        msg << key;
        msg << value;
    }

    msg << count;
}

template<typename K, typename V>
inline void UnpackList(net::message<MsgTypes>& msg, std::unordered_map<K, V>& outMap) {
	if( msg.size()==0 ) return;

    uint32_t count = 0;
    msg >> count;
    for (uint32_t i = 0; i < count; ++i) {
        K key;
        V value;
        msg >> value;
        msg >> key;
        outMap[key] = value;
    }
}

template<typename Pool, typename Map>
void kill_if_not_in_map(Pool& pool, const Map& mp, int ignorID = -1)
{
    pool.kill_if([&](const auto& obj)
    {
        uint32_t id = obj->ID();

        // Не видаляти цей ID
        if(id == ignorID)
            return false;

        // Якщо ID немає в мапі → видаляємо
        return mp.find(id) == mp.end();
    });
}

template<typename Vec, typename Map, typename Factory>
void create_missing(Vec& vec, const Map& mp, Factory factory, int ignorID = -1)
{
    for (auto& [id, state] : mp)
    {
        // Пропускаємо ID, який не треба створювати
        if (id == ignorID)
            continue;

        auto it = std::find_if(vec.begin(), vec.end(),
            [&](const auto& o){ return o->ID() == id; });

        if (it == vec.end())
        {
            factory(id, state);
        }
    }
}

inline void kill_if_not_in_worldstate(utility::ObjectPool<std::shared_ptr<StaticObject>>& pool,
					const WorldState& ws, int ignorID = -1){
	pool.kill_if([&](const auto& obj){
		uint32_t id = obj->ID();

		// Локальний гравець ніколи не видаляємо
		if(id == ignorID) return false;

		// Якщо ID є в будь-якій з трьох мап → залишаємо
		if(ws.PlayerRst.find(id) != ws.PlayerRst.end()) return false;
		if(ws.BotsRst.find(id) != ws.BotsRst.end()) return false;
		if(ws.Projectiles.find(id) != ws.Projectiles.end()) return false;

		// Інакше → видаляємо
		return true;
	});
}