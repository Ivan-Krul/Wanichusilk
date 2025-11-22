#pragma once
#include <vector>
#include <array>

#include "define.h"
#include "Clock.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Animation : public ClockHolder {
public:
    inline Animation() = default;
    inline Animation(const char* path, SDL_Renderer* renderer) { create(path, renderer); }
    bool   create(const char* path, SDL_Renderer* renderer);
    
    inline short           getFrameCount()        const noexcept { return mDelays_ms.size(); }
    inline float           getTimeMult()          const noexcept { return mTimeMult; }
    inline Clock::Duration getNextFrameDuration() const noexcept { return isGoing() ? Clock::Duration(std::chrono::milliseconds(mDelays_ms[mFrameIndex])) : Clock::Duration(0); }
    inline Clock::Duration getSumFrameDuration()  const noexcept { return isGoing() ? Clock::Duration(std::chrono::milliseconds(mDelaySum)) : Clock::Duration(0); }
    inline SDL_FRect       getRectRes()           const noexcept { return mRect; }
    inline bool            isGoing()              const noexcept { return mFrameIndex != 1; }

    void start(float time_mult = 1.f);
    void render();
    void finish();

    inline void setFrameMult(float time_mult = 1.f) noexcept { mTimeMult = time_mult; }
    inline void setRectRes(SDL_FRect rect)          noexcept { mRect = rect; }
    

    void   clear();
    inline ~Animation() { if (mpAnimation) IMG_FreeAnimation(mpAnimation); }
private:
    inline bool isBig() { return mpAnimation ? (DEFAULT_ANIM_SLOT_USE_THRESHOLD < mpAnimation->count * mpAnimation->w * mpAnimation->h) : false; }

    IMG_Animation* mpAnimation = NULL;
    SDL_Renderer* mpRendererOrigin = NULL;

    SDL_FRect mRect;
    std::vector<uint16_t> mDelays_ms;
    uint16_t mDelaySum;
    short mFrameIndex = -1;
    float mTimeMult = 1.f;
};
