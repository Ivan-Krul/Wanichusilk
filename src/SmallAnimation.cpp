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

    if (!packAnimationInRendTexture()) {
        SDL_Log("error: %s\n", SDL_GetError());
        return;
    }

    mCurrentDelay = std::chrono::milliseconds(mDelays_ms[0]);
}

void SmallAnimation::render() {
    if (mFrameIndex >= mDelays_ms.size()) return;
    mCurrentDelay -= pClock->DeltaTime();

    SDL_Log("time: %f", mCurrentDelay.count());

    while (mCurrentDelay.count() < 0.f) {
        mFrameIndex++;
        if (mFrameIndex >= mDelays_ms.size() && !mIsLoop) return;
        mFrameIndex %= mDelays_ms.size();
        mCurrentDelay += std::chrono::milliseconds(mDelays_ms[mFrameIndex]);
    }
    //while (mCurrentDelay > std::chrono::milliseconds(mDelays_ms[mFrameIndex])) {
    //    
    //    if (mFrameIndex >= mDelays_ms.size() && !mIsLoop) return;
    //    
    //    mCurrentDelay -= std::chrono::milliseconds(mDelays_ms[mFrameIndex]);
    //}

    // render here
    SDL_FRect src;
    src.w = mpAnimation->w;
    src.h = mpAnimation->h;
    src.x = (mFrameIndex % mRenderTexTileWidth) * src.w;
    src.y = (mFrameIndex / mRenderTexTileWidth) * src.h;

    SDL_RenderTexture(mpRendererOrigin, mpRenderTextureTile, &src, &mRect);
}

void SmallAnimation::setAlpha(uint8_t alpha) noexcept {
    mAlpha = alpha;
    if (mpRenderTextureTile)
        SDL_SetTextureAlphaMod(mpRenderTextureTile, mAlpha);
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

    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint8 a = 0;

    for (int f = 0; f < mDelays_ms.size(); f++) {
        tx = f % mRenderTexTileWidth;
        ty = f / mRenderTexTileWidth;
        for (int x = 0; x < ox; x++) {
            for (int y = 0; y < oy; y++) {
                if (!SDL_ReadSurfacePixel((mpAnimation->frames)[f], x, y, &r, &g, &b, &a))
                    return false;

                if (!SDL_WriteSurfacePixel(surf, tx * ox + x, ty * oy + y, r, g, b, a))
                    return false;
            }
        }
    }

    mpRenderTextureTile = SDL_CreateTextureFromSurface(mpRendererOrigin, surf);
    SDL_DestroySurface(surf);
    return mpRenderTextureTile;
}

void SmallAnimation::findTileResolution() {
    char side = 2;
    while (side < mDelays_ms.size()) {
        float res = (float)mDelays_ms.size() / (float)side;
        if (res - (int)res > 0.f) {
            side++;
        }
        else {
            mRenderTexTileWidth = mDelays_ms.size() / side;
            mRenderTexTileHeight = side;
            return;
        }
    }
    mRenderTexTileHeight = side;
    mRenderTexTileWidth = 1;
}
