#include "SoundManager.h"

void SoundManager::PlayMusic(const std::string& path, bool loop, float volume) {
	std::lock_guard<std::mutex> lock(m_Mutex);

	auto it = m_Music.find(path);
	if (it == m_Music.end()) {
		auto music = std::make_shared<sf::Music>();
		if (!music->openFromFile(path)) return;

		music->setLooping(loop);
		music->setVolume(volume);
		music->play();

		m_Music[path] = music;
	} else {
		it->second->stop();
		it->second->setLooping(loop);
		it->second->setVolume(volume);
		it->second->play();
	}
}

void SoundManager::StopMusic(const std::string& path) {
	std::lock_guard<std::mutex> lock(m_Mutex);

	auto it = m_Music.find(path);
	if (it != m_Music.end()) {
		it->second->stop();
	}
}

void SoundManager::SetMusicLoop(const std::string& path, bool loop) {
	std::lock_guard<std::mutex> lock(m_Mutex);

	auto it = m_Music.find(path);
	if (it != m_Music.end()) {
		it->second->setLooping(loop);
	}
}

void SoundManager::SetMusicVolume(const std::string& path, float volume) {
	std::lock_guard<std::mutex> lock(m_Mutex);

	auto it = m_Music.find(path);
	if (it != m_Music.end()) {
		it->second->setVolume(volume);
	}
}

void SoundManager::PlaySound(const std::string& path, float volume) {
	std::lock_guard<std::mutex> lock(m_Mutex);

	// отримуємо або завантажуємо буфер
	auto it = m_SoundBuffers.find(path);
	if (it == m_SoundBuffers.end()) {
		sf::SoundBuffer buffer;
		if (!buffer.loadFromFile(path)) return;
		m_SoundBuffers[path] = buffer;
		it = m_SoundBuffers.find(path);
	}

	auto sound = std::make_shared<sf::Sound>(it->second);
	sound->setVolume(volume);
	sound->play();

	m_ActiveSounds.push_back(sound);
}
