#include "Animation.h"

bool Animation::create(const char* path, SDL_Renderer* renderer) {
    if (mHasHead && muHandle.anim) return false;
    mpRendererOrigin = renderer;
    muHandle.anim = IMG_LoadAnimation(path);
    if (muHandle.anim == NULL) return false;
    mHasHead = true;
    mIsLoop = false;
    mIsFreezed = false;

    mRect.w = muHandle.anim->w;
    mRect.h = muHandle.anim->h;

    mDelaySum = 0;
    mDelays_ms.resize(muHandle.anim->count);
    for (int i = 0; i < muHandle.anim->count; i++) {
        mDelays_ms[i] = muHandle.anim->delays[i];
        mDelaySum += mDelays_ms[i];
    }

    return muHandle.anim;
}

bool Animation::create(Animation&& instance) noexcept {
    mpRendererOrigin = instance.mpRendererOrigin;
    muHandle = instance.muHandle;
    mDelaySum = instance.mDelaySum;
    mDelays_ms = instance.mDelays_ms;
    mRect = instance.mRect;
    mFrameIndex = instance.mFrameIndex;
    mTimeMult = instance.mTimeMult;
    mIsLoop = instance.mIsLoop;
    mHasHead = instance.mHasHead;
    if(!pClock) pClock = instance.pClock;

    instance.mpRendererOrigin = nullptr;
    instance.muHandle.anim = nullptr;
    return muHandle.anim;
}

void Animation::finish() {
    mFrameIndex = -1;
}

void Animation::lockChange() {
    if (muHandle.anim && mHasHead)
        IMG_FreeAnimation(muHandle.anim);
    mHasHead = false;
}

void Animation::clear() {
    if (muHandle.anim && mHasHead)
        IMG_FreeAnimation(muHandle.anim);

    mDelays_ms.clear();

    mHasHead = true;
    muHandle.anim = NULL;

    childClean();
}

bool Animation::preRender() {
    if (mFrameIndex >= mDelays_ms.size()) return true;
    if (mIsFreezed) return false;
    mCurrentDelay -= pClock->DeltaTime();

    while (mCurrentDelay.count() < 0.f) {
        mFrameIndex++;
        if (mFrameIndex >= mDelays_ms.size() && !mIsLoop) return true;
        mFrameIndex %= mDelays_ms.size();
        mCurrentDelay += std::chrono::milliseconds(mDelays_ms[mFrameIndex]);
    }

    return false;
}

