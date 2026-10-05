#pragma once

#include <unordered_map>
#include <algorithm>
#include <ImplSingleton.h>

enum class PlayerConnState { Connecting, Loading, InMatch, Disconnected };

struct PlayerSession {
    uint32_t id = 0;
    PlayerConnState state = PlayerConnState::Connecting;
    bool bReady = false;
};

class MatchState {
    public:

    std::unordered_map<uint32_t, PlayerSession> players; // багато гравців → мапа, і тому ImplSingleton, а не поле одного об'єкта

    size_t PlayersInMatch() const {
        return std::count_if(players.begin(), players.end(),
            [](auto& p){ return p.second.state == PlayerConnState::InMatch; });
    }
};