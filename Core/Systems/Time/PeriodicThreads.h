#pragma once

#include "PreciseTimer.h"
#include <algorithm>
#include <functional>
#include <stop_token>
#include <thread>
#include <vector>

class PeriodicThreads {
public:
    using Clock = std::chrono::steady_clock;
    using Task  = std::function<void(float dt)>;

    PeriodicThreads() = default;
    ~PeriodicThreads() { RequestStop(); }               // jthreads join after this

    PeriodicThreads(const PeriodicThreads&)            = delete;
    PeriodicThreads& operator=(const PeriodicThreads&) = delete;

    // Register before Start(). Every task gets its own thread.
    void AddTask(double hz, Task task) {
        if (m_vThreads.empty())
            m_vTasks.push_back({ hz, std::move(task) });
    }

    void Start() {
        if (!m_vThreads.empty()) return;                // already started
        for (const auto& t : m_vTasks)
            m_vThreads.emplace_back([this, &t] { ThreadMain(t); });
    }

    // Safe from anywhere, including from inside a task
    void RequestStop() { m_xStop.request_stop(); }

    bool IsRunning() const { return !m_xStop.stop_requested(); }

private:
    struct Entry {
        double hz;
        Task   task;
    };

    void ThreadMain(const Entry& e) {
        using Sec = std::chrono::duration<double>;

        const auto  stop     = m_xStop.get_token();
        const auto  interval = std::chrono::duration_cast<Clock::duration>(Sec(1.0 / e.hz));
        const float maxDt    = static_cast<float>(4.0 / e.hz);   // clamp after long stalls

        PreciseTimer timer;
        auto last = Clock::now();
        auto next = last + interval;

        while (!stop.stop_requested()) {
            timer.SleepUntil(next);
            if (stop.stop_requested()) break;

            const auto now = Clock::now();
            e.task(std::min(std::chrono::duration<float>(now - last).count(), maxDt));
            last = now;

            next += interval;
            if (const auto after = Clock::now(); after >= next) next = after + interval;  // drop backlog
        }
    }

    std::stop_source         m_xStop;
    std::vector<Entry>       m_vTasks;                  // frozen after Start()
    std::vector<std::jthread> m_vThreads;               // declared last: destroyed (joined) first
};