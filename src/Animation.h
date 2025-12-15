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
    inline Animation(Animation&& instance) noexcept { create(std::move(instance)); }
    inline Animation(const char* path, SDL_Renderer* renderer) { create(path, renderer); }
    bool   create(Animation&& instance) noexcept;
    bool   create(const char* path, SDL_Renderer* renderer);
    
    inline short           getFrameCount()        const noexcept { return mDelays_ms.size(); }
    inline float           getTimeMult()          const noexcept { return mTimeMult; }
    inline Clock::Duration getNextFrameDuration() const noexcept { return isGoing() ? Clock::Duration(std::chrono::milliseconds(mDelays_ms[mFrameIndex])) : Clock::Duration(0); }
    inline Clock::Duration getSumFrameDuration()  const noexcept { return isGoing() ? Clock::Duration(std::chrono::milliseconds(mDelaySum)) : Clock::Duration(0); }
    inline SDL_FRect       getRectRes()           const noexcept { return mRect; }
    inline uint8_t         getAlpha()             const noexcept { return mAlpha; }
    inline bool            isGoing()              const noexcept { return mFrameIndex != 1; }
    inline virtual bool    isBig()                const noexcept { return DEFAULT_ANIM_SLOT_USE_THRESHOLD < (mpAnimation ? (mpAnimation->count * mpAnimation->w * mpAnimation->h) : 0); }
    inline bool            isLoop()               const noexcept { return mIsLoop; }

    virtual void start(float time_mult = 1.f) = 0;
    virtual void render() = 0;
    void finish();

    inline void setFrameMult(float time_mult = 1.f) noexcept { mTimeMult = time_mult; }
    inline void setRectRes(SDL_FRect rect)          noexcept { mRect = rect; }
    inline void setLooping(bool need_loop)          noexcept { mIsLoop = need_loop; }
    inline virtual void setAlpha(uint8_t alpha)     noexcept = 0;
    

    void   clear();
    inline virtual ~Animation() { if (mpAnimation) IMG_FreeAnimation(mpAnimation); }

protected:
    bool preRender();

protected:
    static const SDL_PixelFormat cPixelFormat = SDL_PIXELFORMAT_RGBA32;

protected:
    IMG_Animation* mpAnimation = NULL;
    SDL_Renderer* mpRendererOrigin = NULL;

    SDL_FRect mRect = { 0.f };
    std::vector<uint16_t> mDelays_ms;
    uint16_t mDelaySum;
    short mFrameIndex = -1;
    float mTimeMult = 1.f;

    uint8_t mAlpha = 255;
    bool mIsLoop = false;

    Clock::Duration mCurrentDelay;


};
