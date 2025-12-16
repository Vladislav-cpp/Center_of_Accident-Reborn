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


class GameTime {
	public:
	GameTime() : m_delta(0.f), m_total(0.f) {
		m_clock.restart();
	}

	// Викликати щораз на початку нового кадру
	void Update() {
		float now = m_clock.getElapsedTime().asSeconds();
		m_delta = now - m_total;
		m_total = now;
	}

	// Повертає дельту часу з останнього виклику Update (в секундах)
	float Delta() const { return m_delta; }

	// Повертає час з моменту створення або останнього Reset
	float TotalElapsed() const { return m_total; }

	// Перезапускати таймер
	void Reset() {
		m_clock.restart();
		m_delta = 0.f;
		m_total = 0.f;
	}

	private:
	sf::Clock m_clock;
	float m_delta;     // дельта часу між кадрами
	float m_total;     // загальний пройдений час
};
