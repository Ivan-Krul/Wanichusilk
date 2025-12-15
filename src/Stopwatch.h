#pragma once
#include "define.h"
#include "Clock.h"

#ifdef DEBUG
struct ScopedStopwatch {
    Clock::SteadyClock::time_point from = Clock::SteadyClock::now();
    const char* name;
    ScopedStopwatch(const char* name) {
        this->name = name;
    }
    ~ScopedStopwatch() {
        SDL_Log("stopwatch %s: %zu ms\n", name, std::chrono::duration_cast<std::chrono::milliseconds>(Clock::SteadyClock::now() - from).count());
    }
};

#define SCOPED_STOPWATCH(x)\
ScopedStopwatch ssw(x)
#else
#define SCOPED_STOPWATCH(x)
#endif
