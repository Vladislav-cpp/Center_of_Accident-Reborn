#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

class GameTimer {
	public:
	explicit GameTimer(float durationSeconds = 0.f) : m_duration(durationSeconds) {
		m_clock.restart();
	}

	// Перезапустити таймер (можна задати нову тривалість)
	void Restart(float durationSeconds = -1.f) {
		if(durationSeconds >= 0.f) m_duration = durationSeconds;
		m_clock.restart();
	}

	// Повертає, скільки часу пройшло
	float Elapsed() const {
		return m_clock.getElapsedTime().asSeconds();
	}

	// Повертає, скільки часу залишилося до кінця
	float Remaining() const {
		float rem = m_duration - Elapsed();
		return rem > 0.f ? rem : 0.f;
	}

	// Тривалість таймера
	float Duration() const { return m_duration; }

	// true, якщо час вичерпано
	bool IsFinished() const {
		return Elapsed() >= m_duration;
	}

	private:
	sf::Clock m_clock;
	float m_duration; // в секундах
};