#pragma once
#include <chrono>

#include <SDL3/SDL.h>

class Clock {
public:
    using Duration = std::chrono::duration<float>;
    using SteadyClock = std::chrono::steady_clock;

    inline void StartMeasure() { mBegin = SteadyClock::now(); }
    inline void FinishMeasure();
    inline void FinishMeasure(float targetFps);
    inline Duration DeltaTime() const { return mDeltaTime; }
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

