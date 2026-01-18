#include "Animation.h"
#include "Logger.h"

bool Animation::baseCreate(const char* path, SDL_Renderer* renderer) {
    if (mHasHead && muHandle.anim) return true;
    mpRendererOrigin = renderer;
    muHandle.anim = IMG_LoadAnimation(path);
    if (muHandle.anim == NULL) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return true;
    }
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

    return false;
}

bool Animation::create(Animation&& instance) noexcept {
    muHandle = instance.muHandle;
    mpRendererOrigin = instance.mpRendererOrigin;

    mRect = instance.mRect;
    mDelays_ms = instance.mDelays_ms;
    mDelaySum = instance.mDelaySum;
    mFrameIndex = instance.mFrameIndex;
    mTimeMult = mTimeMult;
    mAlpha = instance.mAlpha;
    mIsLoop = instance.mIsLoop;
    mHasHead = instance.mHasHead;
    mIsFreezed = instance.mIsFreezed;
    mCurrentDelay = instance.mCurrentDelay;
    if(!pClock) pClock = instance.pClock;

    instance.mpRendererOrigin = nullptr;
    instance.muHandle.anim = nullptr;
    return muHandle.anim;
}

void Animation::start(float time_mult) {
    if (!pClock) {
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "Animation didn't got a clock.");
        return;
    }

    mCurrentDelay = std::chrono::milliseconds(mDelays_ms[0]);
    mFrameIndex = 0;
    mTimeMult = time_mult;
}

void Animation::finish() {
    mFrameIndex = -1;
}

void Animation::lockChange() {
    if (muHandle.anim && mHasHead) {
        IMG_FreeAnimation(muHandle.anim);
        mHasHead = false;
    }
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
    if (!(mTimeMult > 0.f)) return false;
    mCurrentDelay -= pClock->DeltaTime();

    while (mCurrentDelay.count() < 0.f) {
        mFrameIndex++;
        if (mFrameIndex >= mDelays_ms.size() && !mIsLoop) return true;
        mFrameIndex %= mDelays_ms.size();
        mCurrentDelay += std::chrono::milliseconds(mDelays_ms[mFrameIndex]) * mTimeMult;
    }

    return false;
}

