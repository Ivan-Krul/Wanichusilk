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
            bool create(Animation&& instance) noexcept;
    virtual bool create(const char* path, SDL_Renderer* renderer) { return baseCreate(path, renderer); }

    inline IMG_Animation* getAnimationPtr()      const noexcept { return mState.has_head ? muHandle.anim : nullptr; }
    inline size_t          getFrameCount()        const noexcept { return mDelays_ms.size(); }
    inline float           getTimeMult()          const noexcept { return mTimeMult; }
    inline Clock::Duration getNextFrameDuration() const noexcept { return isGoing() ? Clock::Duration(std::chrono::milliseconds(mDelays_ms[mFrameIndex])) : Clock::Duration(0); }
    inline Clock::Duration getSumFrameDuration()  const noexcept { return isGoing() ? Clock::Duration(std::chrono::milliseconds(mDelaySum)) : Clock::Duration(0); }
    inline SDL_FRect       getRectSize()          const noexcept { return SDL_FRect{ 0, 0, (float)(getSizeWidth()), (float)(getSizeHeight()) }; }
    inline SDL_FRect       getRectRes()           const noexcept { return mRect; }
    inline uint8_t         getAlpha()             const noexcept { return mAlpha; }
    inline bool            isGoing()              const noexcept { return mFrameIndex != 1; }
    inline virtual bool    isBig()                const noexcept { return DEFAULT_ANIM_SLOT_USE_THRESHOLD < (muHandle.anim ? (mDelays_ms.size() * getSizeWidth() * getSizeHeight()) : 0); }
    inline bool            isLoop()               const noexcept { return mState.is_loop; }
    inline bool            isPreprocessed()       const noexcept { return mState.is_preprocessed; }
    inline bool            isFreezed()            const noexcept { return mState.is_freezed; }

    virtual void preprocess() { assert("false"); }
    void start(float time_mult = 1.f);
    virtual void render() {}
    virtual void renderRaw(const SDL_FRect* rect, const uint8_t alpha = 255, const float time_mult = 1.f) {}
    inline void finish();

    inline void setFrameMult(float time_mult = 1.f) noexcept { mTimeMult = time_mult; }
    inline void setRectRes(SDL_FRect rect)          noexcept { mRect = rect; }
    inline void setLooping(bool need_loop)          noexcept { mState.is_loop = need_loop; }
    inline void setFreeze(bool freeze)              noexcept { mState.is_freezed = freeze; }
    inline virtual void setAlpha(uint8_t alpha)     noexcept {}

    void lockChange();

    void   clear();
    inline virtual ~Animation() { if (muHandle.anim && mState.has_head) IMG_FreeAnimation(muHandle.anim);}

protected:
    inline int16_t getSizeWidth() const noexcept { return mState.has_head ? muHandle.anim->w : muHandle.size.width; }
    inline int16_t getSizeHeight() const noexcept { return mState.has_head ? muHandle.anim->h : muHandle.size.height; }

    bool preRender();

    inline virtual void childClean() {}
    bool baseCreate(const char* path, SDL_Renderer* renderer);

protected:
    static const SDL_PixelFormat cPixelFormat = SDL_PIXELFORMAT_RGBA32;

protected:
    union PictureMap {
        SDL_Surface* surf = nullptr;
        SDL_Texture* tex;
    };

    union {
        IMG_Animation* anim = NULL;
        struct {
            int16_t width;
            int16_t height;
        } size;
    } muHandle;
    SDL_Renderer* mpRendererOrigin = NULL;

    SDL_FRect mRect = { 0.f };
    std::vector<uint16_t> mDelays_ms;
    uint16_t mDelaySum = 0; // not longer than 65 seconds
    int16_t mFrameIndex = -1;
    float mTimeMult = 1.f;

    uint8_t mAlpha = 255;
    struct {
        bool is_loop : 1;
        bool has_head : 1;
        bool is_freezed : 1;
        bool is_preprocessed : 1;
    } mState = { 0 };

    Clock::Duration mCurrentDelay = Clock::Duration::zero();


};
