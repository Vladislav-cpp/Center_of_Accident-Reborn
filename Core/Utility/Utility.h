#pragma once
#include <utility>
#include <memory>
#include <vector>
#include "cute_c2.h"
#include <SFML/Graphics.hpp>
#include "IStats.h"


namespace utility {

template<typename T>
struct ObjectPool {

	T& push_back(T&& obj) {
		if(alive < data.size())	data[alive] = std::move(obj);
		else data.push_back(std::move(obj));
		return data[alive++];
	}

	T& push_back(const T& obj) {
		if(alive < data.size())	data[alive] = obj;
		else data.push_back(obj);
		return data[alive++];
	}

	void kill_at(size_t i) {
		std::swap(data[i], data[alive - 1]);
		--alive;
	}

	template<typename Pred>
	void kill_if(Pred&& pred) {
		for(size_t i = 0; i < alive; ) {
			if(pred(data[i])) kill_at(i);
			else ++i;
		}
	}

	// swap
	// does not preserve the order of elements
	void kill(const T& val) {
		kill_at( size_t(&val - &data[0]) );
	}

	auto begin() { return data.begin(); }
	auto end() { return data.begin() + alive; }
	auto begin() const { return data.begin(); }
	auto end() const { return data.begin() + alive; }

	T& operator[](size_t i) { return data[i]; }
	const T& operator[](size_t i) const { return data[i]; }

	void resize(size_t n) {
		data.resize(n);
		alive = n;
	}

	size_t size() const { return alive; }
	bool empty() const { return alive == 0; }

	std::vector<T> data;
	size_t alive = 0;
};

template<typename T>
void KillByIDs(ObjectPool<T>& pool, const std::vector<int>& ids)
{
    if (ids.empty() || pool.empty())
        return;

    pool.kill_if([&](const T& obj) {
        const int objID = obj->ID(); // ÿךשמ T = shared_ptr<Something>
        return std::find(ids.begin(), ids.end(), objID) != ids.end();
    });
}



// swap + pop_back
// does not preserve the order of elements
template<typename T, typename Pred>
void RemoveIf(std::vector<std::shared_ptr<T>>& vec, Pred pred) {
	for (size_t i = 0; i < vec.size(); ) if( pred(vec[i]) ) {
		std::swap(vec[i], vec.back());
		vec.pop_back();
	} else ++i;
}


std::pair<float, float> normalize(float x, float y);

std::string GetExecutablePath();

void DrawDebugCircle( sf::RenderWindow& window, const sf::Sprite& sprite );
void DrawDebugCapsule(sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color color = sf::Color::Red);
void DrawDebugRect(sf::RenderWindow& window, const sf::Sprite& sprite);
// generic version
void DrawDebug( sf::RenderWindow& window, const sf::Sprite& sprite, const IStaticStatsBase& stat );

sf::Vector2f ClampToMap(const sf::Vector2f& pos, float viewWidth, float viewHeight, float mapWidth, float mapHeight);

//
std::array<sf::Vector2f, 4> GetSpriteGlobalQuad(const sf::Sprite& spr);
c2Circle GetCircle(const sf::Sprite& spr);
c2AABB GetAABB(const sf::Sprite& spr);
c2Capsule GetCapsule(const sf::Sprite& spr);
c2Poly GetQuadPoly(const sf::Sprite& spr);

} // namespace utility