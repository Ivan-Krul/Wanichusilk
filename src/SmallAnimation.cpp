#include "SmallAnimation.h"
#include "Logger.h"

SmallAnimation::SmallAnimation(Animation&& inst, char& is_fail) {
    if (is_fail = create(std::move(inst))) return;

    is_fail = packAnimationInSingleSurface();
}

bool SmallAnimation::create(const char* path, SDL_Renderer* renderer) {
    if(baseCreate(path, renderer)) return true;

    if (packAnimationInSingleSurface()) return true;

    return false;
}

bool SmallAnimation::preprocess() {
    SDL_Surface* surf = mpTiles.surf;
    mpTiles.tex = SDL_CreateTextureFromSurface(mpRendererOrigin, surf);
    if (!mpTiles.tex) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logWarningIn(__FUNCTION__, "Convertion to texture was failed.");
        Logger log_sdl(DEFAULT_LOG_SDL_PATH);
        log_sdl.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return true;
    }

    SDL_DestroySurface(surf);

    if (mpTiles.tex && mAlpha != 255)
        SDL_SetTextureAlphaMod(mpTiles.tex, mAlpha);

    mState.is_preprocessed = true;
    return false;
}

void SmallAnimation::render() {
    if (preRender()) return;
    if (mpTiles.tex == nullptr) {
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "Texture tile wasn't initialised.");
        return;
    }

    // render here
    mSrcRect.x = (mFrameIndex % mTileWidth) * mSrcRect.w;
    mSrcRect.y = (mFrameIndex / mTileWidth) * mSrcRect.h;

    SDL_RenderTexture(mpRendererOrigin, mpTiles.tex, &mSrcRect, &mRect);
}

void SmallAnimation::renderRaw(const SDL_FRect* rect, const uint8_t alpha, const float time_mult) {
    const float time_mult_was = mTimeMult;
    mTimeMult = time_mult;

    if (preRender()) {
        mTimeMult = time_mult_was;
        return;
    }
    if (mpTiles.tex == nullptr) {
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "Texture tile wasn't initialised.");
        mTimeMult = time_mult_was;
        return;
    }

    mSrcRect.x = (mFrameIndex % mTileWidth) * mSrcRect.w;
    mSrcRect.y = (mFrameIndex / mTileWidth) * mSrcRect.h;

    SDL_SetTextureAlphaMod(mpTiles.tex, alpha);

    SDL_RenderTexture(mpRendererOrigin, mpTiles.tex, &mSrcRect, rect);

    SDL_SetTextureAlphaMod(mpTiles.tex, mAlpha);
    mTimeMult = time_mult_was;
}

void SmallAnimation::setAlpha(uint8_t alpha) noexcept {
    mAlpha = alpha;
    if (mpTiles.tex) SDL_SetTextureAlphaMod(mpTiles.tex, mAlpha);
}

void SmallAnimation::childClean() {
    if(mpTiles.tex) SDL_DestroyTexture(mpTiles.tex);
}

bool SmallAnimation::packAnimationInSingleSurface() {
    if (!muHandle.anim || !mState.has_head) {
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "Animation won't start because of miss-match of heads.");
        log.logInfoIn(__FUNCTION__, "has_head: %d.", mState.has_head);
        return true;
    }

    findTileResolution();
    mpTiles.surf = SDL_CreateSurface(muHandle.anim->w * mTileWidth, muHandle.anim->h * mTileHeight, cPixelFormat);
    if (!mpTiles.surf) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__,  "Error with surface %s.", SDL_GetError());
        return true;
    }

    const auto ox = muHandle.anim->w;
    const auto oy = muHandle.anim->h;

    mSrcRect.w = ox;
    mSrcRect.h = oy;

    char tx = 0;
    char ty = 0;

    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint8 a = 0;

    for (int f = 0; f < mDelays_ms.size(); f++) {
        tx = f % mTileWidth;
        ty = f / mTileWidth;
        for (int x = 0; x < ox; x++) {
            for (int y = 0; y < oy; y++) {
                if (!SDL_ReadSurfacePixel((muHandle.anim->frames)[f], x, y, &r, &g, &b, &a))
                    return true;

                if (!SDL_WriteSurfacePixel(mpTiles.surf , tx * ox + x, ty * oy + y, r, g, b, a))
                    return true;
            }
        }
    }

    return false;
}

void SmallAnimation::findTileResolution() {
    char side = 2;
    while (side < mDelays_ms.size()) {
        float res = (float)mDelays_ms.size() / (float)side;
        if (res - (int)res > 0.f) {
            side++;
        }
        else {
            mTileWidth = mDelays_ms.size() / side;
            mTileHeight = side;
            return;
        }
    }
    mTileHeight = side;
    mTileWidth = 1;
}
