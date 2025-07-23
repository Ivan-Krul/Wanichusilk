#pragma once
#include "easefunc.h"
#include "Clock.h"
#include "FilmKeypoint.h"

#include <functional>

template<typename Func = float(*)(float)>
class EaseTracker : public ClockHolder {
public:
    static_assert(std::is_function<Func>::value, "use the function type for template");

    static constexpr float c_ease_no_progress = -0.f;
    static constexpr float c_ease_use_default = 2.f;

    inline void setDefault() { mProgress = c_ease_use_default; }
    inline void setEase(Func ease) { fEaseFunction = ease; }

    inline void reset() { mProgress = c_ease_no_progress; }

    inline void start(FilmTimer timer) { setTimer(timer); mProgress = 0.f; }
    inline void start(int frames) { setTimer(frames); mProgress = 0.f; }
    inline void start(Clock::Duration duration) { setTimer(duration); mProgress = 0.f; }
    inline void start() { mProgress = 0.f; }
    
    void update();

    inline float isDefault() const { return mDefault == c_ease_use_default; }
    inline float isProgress() const { return mDefault != c_ease_no_progress && mDefault != c_ease_use_default; }
    inline float getProgress() const { return mProgress; }

    inline operator float() const { return fEaseFunction ? fEaseFunction(mProgress) : mProgress; }

private:
    void setTimer(FilmTimer timer);
    void setTimer(int frames);
    void setTimer(Clock::Duration duration);
        
    Func fEaseFunction;
    FilmTimer mTimerLimiter;
    FilmTimer mTimerRunner;

    float mProgress = c_ease_use_default;
};
