#include "SmallAnimation.h"

void SmallAnimation::start(float time_mult) {
    if (!mpAnimation) return;
    mFrameIndex = 0;
    mTimeMult = time_mult;

    if (!packAnimationInRendTexture()) {
        SDL_Log("error: %s\n", SDL_GetError());
        return;
    }

    mSrcRect.w = mpAnimation->w;
    mSrcRect.h = mpAnimation->h;
    mCurrentDelay = std::chrono::milliseconds(mDelays_ms[0]);
}

void SmallAnimation::render() {
    if (preRender()) return;

    // render here
    mSrcRect.x = (mFrameIndex % mRenderTexTileWidth) * mSrcRect.w;
    mSrcRect.y = (mFrameIndex / mRenderTexTileWidth) * mSrcRect.h;

    SDL_RenderTexture(mpRendererOrigin, mpRenderTextureTile, &mSrcRect, &mRect);
}

void SmallAnimation::setAlpha(uint8_t alpha) noexcept {
    mAlpha = alpha;
    if (mpRenderTextureTile)
        SDL_SetTextureAlphaMod(mpRenderTextureTile, mAlpha);
}

bool SmallAnimation::packAnimationInRendTexture() {
    findTileResolution();
    SDL_Surface* surf = SDL_CreateSurface(mpAnimation->w * mRenderTexTileWidth, mpAnimation->h * mRenderTexTileHeight, cPixelFormat);
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

    if(mpRenderTextureTile && mAlpha != 255)
        SDL_SetTextureAlphaMod(mpRenderTextureTile, mAlpha);

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
