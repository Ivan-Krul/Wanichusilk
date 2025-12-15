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

bool Animation::create(Animation&& instance) noexcept {
    mpRendererOrigin = instance.mpRendererOrigin;
    mpAnimation = instance.mpAnimation;
    mDelaySum = instance.mDelaySum;
    mDelays_ms = instance.mDelays_ms;
    mRect = instance.mRect;
    mFrameIndex = instance.mFrameIndex;
    mTimeMult = instance.mTimeMult;
    mIsLoop = instance.mIsLoop;
    if(!pClock) pClock = instance.pClock;

    instance.mpRendererOrigin = nullptr;
    instance.mpAnimation = nullptr;
    return mpAnimation;
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

bool Animation::preRender() {
    if (mFrameIndex >= mDelays_ms.size()) return true;
    mCurrentDelay -= pClock->DeltaTime();

    while (mCurrentDelay.count() < 0.f) {
        mFrameIndex++;
        if (mFrameIndex >= mDelays_ms.size() && !mIsLoop) return true;
        mFrameIndex %= mDelays_ms.size();
        mCurrentDelay += std::chrono::milliseconds(mDelays_ms[mFrameIndex]);
    }

    return false;
}

