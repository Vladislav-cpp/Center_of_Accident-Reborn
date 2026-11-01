#pragma once

// Must come BEFORE <windows.h>: keeps the old winsock.h out so it doesn't clash with Asio
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <timeapi.h>

#include <chrono>
#include <thread>

#pragma comment(lib, "winmm.lib")

#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

// One instance per thread (a waitable timer handle must not be shared between sleeping threads)
class PreciseTimer {
public:
    using Clock = std::chrono::steady_clock;

    PreciseTimer() {
        // Windows 11: don't let the OS throttle timer resolution (done once per process)
        static const bool once = [] {
            PROCESS_POWER_THROTTLING_STATE s{};
            s.Version     = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
            s.ControlMask = PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION;
            s.StateMask   = 0;
            SetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, &s, sizeof(s));
            return true;
        }();
        (void)once;

        m_hTimer = CreateWaitableTimerExW(nullptr, nullptr,
            CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_MODIFY_STATE | SYNCHRONIZE);

        if (!m_hTimer) { timeBeginPeriod(1); m_bFallback = true; }   // pre-Windows 10 1803
    }

    ~PreciseTimer() {
        if (m_hTimer)    CloseHandle(m_hTimer);
        if (m_bFallback) timeEndPeriod(1);
    }

    PreciseTimer(const PreciseTimer&)            = delete;
    PreciseTimer& operator=(const PreciseTimer&) = delete;

    void SleepUntil(Clock::time_point target) {
        const auto now = Clock::now();
        if (target <= now) return;

        if (m_hTimer) {
            const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(target - now).count();
            LARGE_INTEGER due;
            due.QuadPart = -(ns / 100);                 // negative = relative, 100 ns units
            if (SetWaitableTimerEx(m_hTimer, &due, 0, nullptr, nullptr, nullptr, 0)) {
                WaitForSingleObject(m_hTimer, INFINITE);
                return;
            }
        }
        std::this_thread::sleep_until(target);
    }

private:
    HANDLE m_hTimer    = nullptr;
    bool   m_bFallback = false;
};