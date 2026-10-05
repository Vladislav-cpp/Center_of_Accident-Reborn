#pragma once
#include <cstdint>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <ImplSingleton.h>

//     Єдине джерело правди про dt поточного кадру/тіка.
//     Пишеться ОДИН раз за кадр/тік, читається скільки завгодно
//     разів через DeltaTime()/TotalTime() — ідемпотентно, без побічних ефектів.
//     
//     Сервер (фіксований крок симуляції, ServerGame::Run()):        TimeSys().Tick(tickTimer.Duration());
//     
//     Клієнт (реальний виміряний кадровий час, ClientGame::Run()):  TimeSys().Tick();

class TimeSystem : public ImplSingleton<TimeSystem> {
	friend class ImplSingleton<TimeSystem>;

	public:
	TimeSystem() { m_clock.restart(); }
	~TimeSystem() = default;

	// Клієнтський режим: виміряти реальний час з попереднього виклику.
	void Tick() {
		float now = m_clock.getElapsedTime().asSeconds();
		m_delta = now - m_total;
		m_total = now;
		++m_tickIndex;
	}

	// Серверний режим: фіксований крок симуляції, а не виміряний.
	void Tick(float fixedDelta) {
		m_delta = fixedDelta;
		m_total += fixedDelta;
		++m_tickIndex;
	}

	float DeltaTime()    const { return m_delta; }
	float TotalTime()    const { return m_total; }
	uint64_t TickIndex() const { return m_tickIndex; }

	void Reset() {
		m_clock.restart();
		m_delta = 0.f;
		m_total = 0.f;
		m_tickIndex = 0;
	}

	private:
	sf::Clock m_clock;
	float m_delta = 0.f;
	float m_total = 0.f;
	uint64_t m_tickIndex = 0;
};

inline TimeSystem& TimeSys() { return TimeSystem::Instance(); }