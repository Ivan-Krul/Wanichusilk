#include "EaseTracker.h"

template<typename Func>
void EaseTracker<Func>::update() {
    if (!isProgress()) return;

    if (mTimerRunner.is_zero()) {
        mProgress = c_ease_end;
        return;
    }

    mTimerRunner.decrement_time_frame(pClock->DeltaTime());
    
    if (!mTimerRunner.need_time_delay)
        mProgress = mTimerRunner.frame_delay / float(mTimerLimiter.frame_delay); // all operations happen there so it won't be x/0
    else
        mProgress = mTimerRunner.delay.count() / float(mTimerLimiter.delay.count()); // same with this

    mProgress = 1.f - mProgress;
}

template<typename Func>
inline void EaseTracker<Func>::setTimer(FilmTimer timer) {
    mTimerLimiter = timer;
    mTimerRunner = timer;
}

template<typename Func>
void EaseTracker<Func>::setTimer(int frames) {
    mTimerLimiter.set_delay_frame(frames);
    mTimerRunner.set_delay_frame(frames);
}

template<typename Func>
void EaseTracker<Func>::setTimer(Clock::Duration duration) {
    mTimerLimiter.set_delay_time(duration);
    mTimerRunner.set_delay_time(duration);
}
