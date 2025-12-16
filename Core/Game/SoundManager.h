#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <memory>
#include <mutex>
#include "ImplSingleton.h"


class SoundManager : public ImplSingleton<SoundManager> {
	friend class ImplSingleton<SoundManager>;

	public:
	static SoundManager& Instance() {
		static SoundManager instance;
		return instance;
	}

	// --- Музика ---
	void PlayMusic(const std::string& path, bool loop = false, float volume = 100.f);
	void StopMusic(const std::string& path);
	void SetMusicLoop(const std::string& path, bool loop);
	void SetMusicVolume(const std::string& path, float volume);

	// --- Звуки ---
	void PlaySound(const std::string& path, float volume = 100.f);

	private:
	SoundManager() {}
	~SoundManager() {}

	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	std::map<std::string, std::shared_ptr<sf::Music>> m_Music;
	std::map<std::string, sf::SoundBuffer> m_SoundBuffers;

	std::list<std::shared_ptr<sf::Sound>> m_ActiveSounds;
	std::mutex m_Mutex; // для потокобезпечності
};

inline SoundManager& SoundMg() { return SoundManager::Instance(); }
