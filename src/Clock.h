#pragma once
#include <chrono>

#include <SDL3/SDL.h>

#ifdef NO_CLOCK_TRACKING
#pragma message("No clock tracking")
#endif

class Clock {
public:
    using Duration = std::chrono::duration<float>;
    using SteadyClock = std::chrono::steady_clock;

    inline void StartMeasure() { mBegin = SteadyClock::now(); }
    inline void FinishMeasure();
    inline void FinishMeasure(float targetFps);
    inline Duration DeltaTime() const { return mDeltaTime; }
    inline SteadyClock::time_point Now() const { return mBegin; }

#ifdef NO_CLOCK_TRACKING
    inline void DStep(Duration delta);
#endif
private:
    Duration mDeltaTime;
    SteadyClock::time_point mBegin = SteadyClock::now();
    SteadyClock::time_point mNow = SteadyClock::now();
};

inline void Clock::FinishMeasure() {
    mNow = SteadyClock::now();
    mDeltaTime = mNow - mBegin;
}

inline void Clock::FinishMeasure(float targetFps) {
    FinishMeasure();

    if (!(targetFps > 0.0f)) return;
    Duration targetFrameTime(1.0f / targetFps);
    Duration sleepDelay = targetFrameTime - mDeltaTime;

    if (!(sleepDelay.count() > 0.0f)) return;
    SDL_Delay(static_cast<Uint32>(sleepDelay.count() * 1000.0f));
    mNow = SteadyClock::now();
    mDeltaTime = mNow - mBegin;
    
}

#ifdef NO_CLOCK_TRACKING
inline void Clock::DStep(Duration delta) {
    mBegin = mNow;
    mNow += std::chrono::duration_cast<std::chrono::steady_clock::duration>(delta);
    mDeltaTime = mNow - mBegin;
}
#endif

struct ClockHolder {
    inline void setClock(Clock* const clock) { pClock = clock; }

protected:
    Clock* pClock = nullptr;
};

