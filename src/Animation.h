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
    
    inline short     getFrameCount() const noexcept { return mDelays_ms.size(); }
    inline SDL_FRect getRectRes() const noexcept { return mRect; }
    inline bool      isGoing() const noexcept { return mFrameIndex != 1; }

    void start();
    void render();
    void finish();

    void   clear();
    inline ~Animation() { clear(); }
private:
    inline bool isBig() { return mpAnimation ? (DEFAULT_ANIM_SLOT_USE_THRESHOLD < mpAnimation->count * mpAnimation->w * mpAnimation->h) : false; }

    IMG_Animation* mpAnimation = NULL;
    SDL_Renderer* mpRendererOrigin = NULL;

    SDL_FRect mRect;
    std::vector<uint16_t> mDelays_ms;
    short mFrameIndex = -1;
};
