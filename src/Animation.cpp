#include "Animation.h"

bool Animation::create(const char* path, SDL_Renderer* renderer) {
    if (mpAnimation) return false;
    mpRendererOrigin = renderer;
    mpAnimation = IMG_LoadAnimation(path);
    if (mpAnimation == NULL) return false;

    mRect.w = mpAnimation->w;
    mRect.h = mpAnimation->h;

    mDelaySum = 0;
    mDelays_ms.resize(mpAnimation->count);
    for (int i = 0; i < mpAnimation->count; i++) {
        mDelays_ms[i] = mpAnimation->delays[i];
        mDelaySum += mDelays_ms[i];
    }

    return mpAnimation;
    //SDL_CreateSurface(mpAnimation->w, mpAnimation->h, SDL_PIXELFORMAT_RGBA32);
}

void Animation::start(float time_mult) {
    if (!mpAnimation) return;
    mFrameIndex = 0;
    mTimeMult = time_mult;
}

void Animation::finish() {
    mFrameIndex = -1;
}

void Animation::clear() {
    if (mpAnimation)
        IMG_FreeAnimation(mpAnimation);

    mDelays_ms.clear();

    mpAnimation = NULL;
}

