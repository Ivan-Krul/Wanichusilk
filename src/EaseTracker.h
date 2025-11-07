#pragma once
#include "easefunc.h"
#include "Clock.h"
#include "FilmKeypoint.h"

template<typename Func = float(*)(float)>
class EaseTracker : public ClockHolder {
public:
    //static_assert(std::is_function<Func>::value, "use the function type for template");

    static constexpr float c_ease_no_progress = -1.f;
    static constexpr float c_ease_use_default = 2.f;
    static constexpr float c_ease_end = 1.f;

    inline void setDefault() { mProgress = c_ease_use_default; }
    inline void setEase(Func ease) { fEaseFunction = ease; }

    inline void reset() { mProgress = c_ease_no_progress; }

    inline void start(TimerStep timer) { setTimer(timer); mProgress = 0.f; }
    inline void start(int frames) { setTimer(frames); mProgress = 0.f; }
    inline void start(Clock::Duration duration) { setTimer(duration); mProgress = 0.f; }
    inline void start() { mProgress = 0.f; }
    
    void update();

    inline bool isEase() const { return fEaseFunction; }
    inline bool isDefault() const { return mProgress == c_ease_use_default; }
    inline bool isProgress() const { return mProgress != c_ease_no_progress && mProgress != c_ease_use_default && mProgress != c_ease_end; }
    inline bool isEnded() const { return mProgress == c_ease_end; }
    inline float getProgress() const { return mProgress; }

    inline TimerStep getLimiter() const { return mTimerLimiter; }

    inline operator float() const { return fEaseFunction ? fEaseFunction(mProgress) : mProgress; }

private:
    void setTimer(TimerStep timer);
    void setTimer(int frames);
    void setTimer(Clock::Duration duration);
        
    Func fEaseFunction = nullptr;
    TimerStep mTimerLimiter;
    TimerStep mTimerRunner;

    float mProgress = c_ease_use_default;
};

template<typename Func>
void EaseTracker<Func>::update() {
    if (!isProgress()) return;

    if (mTimerRunner.is_zero()) {
        mProgress = c_ease_end;
        return;
    }

    assert(pClock != nullptr);
    mTimerRunner.decrement_time_frame(pClock->DeltaTime());

    if (!mTimerRunner.need_time_delay)
        mProgress = mTimerRunner.frame_delay / float(mTimerLimiter.frame_delay); // all operations happen there so it won't be x/0
    else
        mProgress = mTimerRunner.delay.count() / float(mTimerLimiter.delay.count()); // same with this

    mProgress = 1.f - mProgress;
}

template<typename Func>
inline void EaseTracker<Func>::setTimer(TimerStep timer) {
    mTimerLimiter = timer;
    mTimerRunner = timer;
}

template<typename Func>
inline void EaseTracker<Func>::setTimer(int frames) {
    mTimerLimiter.set_delay_frame(frames);
    mTimerRunner.set_delay_frame(frames);
}

template<typename Func>
inline void EaseTracker<Func>::setTimer(Clock::Duration duration) {
    mTimerLimiter.set_delay_time(duration);
    mTimerRunner.set_delay_time(duration);
}

