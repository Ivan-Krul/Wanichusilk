#include "SmallAnimation.h"
#include "SmallAnimation.h"
#include "SmallAnimation.h"

bool SmallAnimation::create(Animation&& instance) {
    mpRendererOrigin = instance.mpRendererOrigin;
    mpAnimation = instance.mpAnimation;
    mDelaySum = instance.mDelaySum;
    mDelays_ms = instance.mDelays_ms;
    mRect = instance.mRect;
    mFrameIndex = instance.mFrameIndex;
    mTimeMult = instance.mTimeMult;

    instance.mpRendererOrigin = nullptr;
    instance.mpAnimation = nullptr;
    return mpAnimation;
}

void SmallAnimation::start(float time_mult) {
    if (!mpAnimation) return;
    mFrameIndex = 0;
    mTimeMult = time_mult;

    packAnimationInRendTexture();

    mCurrentDelay = std::chrono::milliseconds(mDelays_ms[0]);
}

void SmallAnimation::render() {
    if (mFrameIndex >= mDelays_ms.size()) return;
    auto dt = pClock->DeltaTime();

    mCurrentDelay -= dt;

    while (mCurrentDelay.count() < 0.f) {
        mFrameIndex++;
        if (mFrameIndex >= mDelays_ms.size()) return;
        mCurrentDelay += std::chrono::milliseconds(mDelays_ms[mFrameIndex]);
    }

    // render here

    mRenderTexTileWidth;
    mRenderTexTileHeight;
}

bool SmallAnimation::packAnimationInRendTexture() {
    findTileResolution();
    SDL_Surface* surf = SDL_CreateSurface(mpAnimation->w * mRenderTexTileWidth, mpAnimation->h * mRenderTexTileHeight, SDL_PIXELFORMAT_RGBA32);
    if (!surf)
        return false;

    const auto ox = mpAnimation->w;
    const auto oy = mpAnimation->h;

    char tx = 0;
    char ty = 0;

    Uint8* r = nullptr;
    Uint8* g = nullptr;
    Uint8* b = nullptr;
    Uint8* a = nullptr;

    for (int f = 0; f < mDelays_ms.size(); f++) {
        tx = f % mRenderTexTileWidth;
        ty = f / mRenderTexTileHeight;
        for (int x = 0; x < ox; x++) {
            for (int y = 0; y < oy; y++) {
                if (!SDL_ReadSurfacePixel(mpAnimation->frames[f], x, y, r, g, b, a))
                    return false;

                if (!SDL_WriteSurfacePixel(surf, tx * ox + x, ty * oy + y, r == NULL ? 0 : *r, g == NULL ? 0 : *g, b == NULL ? 0 : *b, a == NULL ? 255 : *a))
                    return false;
            }
        }
    }
    return true;
}

void SmallAnimation::findTileResolution() {
    char side = 2;
    while (side < mDelays_ms.size()) {
        float res = (float)side / (float)mDelays_ms.size();
        if (res - (int)res > 0.f) {
            side++;
        }
        else {
            mRenderTexTileWidth = side / mDelays_ms.size();
            mRenderTexTileHeight = side;
            return;
        }
    }
    mRenderTexTileHeight = side;
    mRenderTexTileWidth = 1;
}
