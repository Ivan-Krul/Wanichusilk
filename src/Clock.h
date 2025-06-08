#pragma once
#include <chrono>

#include <SDL3/SDL.h>

class Clock {
public:
    using Duration = std::chrono::duration<float>;
    using SteadyClock = std::chrono::steady_clock;

    inline void Update();
    inline Duration DeltaTime() const { return mDeltaTime; }
    inline Duration FrameDelay(float targetFps) const;

private:
    Duration mBetweenUpdateDelay;
    Duration mDeltaTime;
    SteadyClock::time_point mBegin = SteadyClock::now();
    SteadyClock::time_point mNow = SteadyClock::now();
};

void Clock::Update() {
    mNow = SteadyClock::now();
    mDeltaTime = mNow - mBegin;
    mBegin = mNow;
}

Clock::Duration Clock::FrameDelay(float targetFps) const {
    Duration targetFrameDuration{ 1.0f / targetFps };
    Duration delay = targetFrameDuration - mDeltaTime;
    return delay > Duration::zero() ? delay : Duration::zero();
}
