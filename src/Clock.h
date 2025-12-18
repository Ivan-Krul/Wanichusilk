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
    Duration mDeltaTime = Duration::zero();
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


struct TimerStep {
    using Duration = std::chrono::duration<float>;

    enum ActionConcurency : unsigned int {
        Instant = 0,
        First,
        Await,
        InInputOrAwait,
        InInputOrFirst,
        InInputAfterAwait,
        InInputAfterFirst,
        Exact
    };

    Duration delay;
    int frame_delay : 28;
    int need_time_delay : 1;
    ActionConcurency action : 3;

    inline TimerStep() : delay(std::chrono::seconds(0)), frame_delay(0), need_time_delay(0), action(Instant) {}

    inline bool is_zero() const { return delay <= delay.zero() && frame_delay <= 0; }
    inline void set_delay_frame(int frames) { frame_delay = frames; need_time_delay = false; }
    inline void set_delay_time(Duration dur) { delay = dur; need_time_delay = true; }
    inline void decrement_frame(int diff_frame) { frame_delay--; }
    inline void decrement_time(Duration diff) { delay -= diff; }
    inline void decrement_time_frame(Duration diff) { need_time_delay ? (delay -= diff, frame_delay--) : (frame_delay--); }

};

namespace clockfunc {
    inline TimerStep min(const TimerStep a, const TimerStep b) {
        TimerStep timer;
        timer.frame_delay = std::min<>(a.frame_delay, b.frame_delay);
        timer.delay = std::min<>(a.delay, b.delay);
        if (timer.delay != timer.delay.zero()) timer.need_time_delay = true;
        return timer;
    }

    inline TimerStep max(const TimerStep a, const TimerStep b) {
        TimerStep timer;
        timer.frame_delay = std::max<>(a.frame_delay, b.frame_delay);
        timer.delay = std::max<>(a.delay, b.delay);
        if (timer.delay != timer.delay.zero()) timer.need_time_delay = true;
        return timer;
    }

#ifdef DEBUG
    inline void SDL_Log_FilmTimer(const TimerStep other) {
        switch (other.action) {
        case TimerStep::Instant: SDL_Log("TimerStep: %fs or %d frames with Instant", other.delay.count(), other.frame_delay); break;
        case TimerStep::First: SDL_Log("TimerStep: %fs or %d frames with First", other.delay.count(), other.frame_delay); break;
        case TimerStep::Await: SDL_Log("TimerStep: %fs or %d frames with Await", other.delay.count(), other.frame_delay); break;
        case TimerStep::InInputOrAwait: SDL_Log("TimerStep: %fs or %d frames with InInputOrAwait", other.delay.count(), other.frame_delay); break;
        case TimerStep::InInputOrFirst: SDL_Log("TimerStep: %fs or %d frames with InInputOrFirst", other.delay.count(), other.frame_delay); break;
        case TimerStep::InInputAfterAwait: SDL_Log("TimerStep: %fs or %d frames with InInputAfterAwait", other.delay.count(), other.frame_delay); break;
        case TimerStep::InInputAfterFirst: SDL_Log("TimerStep: %fs or %d frames with InInputAfterFirst", other.delay.count(), other.frame_delay); break;
        case TimerStep::Exact: SDL_Log("TimerStep: %fs or %d frames with Exact", other.delay.count(), other.frame_delay); break;
        }
    }
#else // inlining nothing should be enough to not generate code for logging in release
    inline void SDL_Log_FilmTimer(const TimerStep other) {}
#endif
}

