#include "BigAnimation.h"
#include "Logger.h"

void BigAnimation::start(float time_mult) {
    if (!muHandle.anim || !mHasHead) {
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "Animation won't start because of miss-match of heads.");
        log.logInfoIn(__FUNCTION__, "has_head: %d.", mHasHead);
        return;
    }
    if (!pClock) {
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "Animation didn't got a clock.");
        return;
    }
    mFrameIndex = 0;
    mTimeMult = time_mult;

    SDL_Surface* surf;

    mapTextures.resize(mDelays_ms.size());
    for (auto f = 0; f < mapTextures.size(); f++) {
        surf = SDL_ConvertSurface(muHandle.anim->frames[f], cPixelFormat);
        mapTextures[f] = SDL_CreateTextureFromSurface(mpRendererOrigin, surf);

        if (mapTextures[f]) {
            if (mAlpha != 255)
                SDL_SetTextureAlphaMod(mapTextures[f], mAlpha);
        }
        else {
            Logger log(DEFAULT_LOG_PATH);
            log.logWarningIn(__FUNCTION__, "Convertion to texture was failed.");
        }

        SDL_DestroySurface(surf);
    }

    mCurrentDelay = std::chrono::milliseconds(mDelays_ms[0]);
}

void BigAnimation::render() {
    if (preRender()) return;

    SDL_RenderTexture(mpRendererOrigin, mapTextures[mFrameIndex], nullptr, &mRect);
}

void BigAnimation::renderRaw(const SDL_FRect* rect, const uint8_t alpha, const float time_mult) {
    const float time_mult_was = mTimeMult;
    mTimeMult = time_mult;

    if (preRender()) return;

    SDL_SetTextureAlphaMod(mapTextures[mFrameIndex], alpha);

    SDL_RenderTexture(mpRendererOrigin, mapTextures[mFrameIndex], nullptr, rect);

    SDL_SetTextureAlphaMod(mapTextures[mFrameIndex], mAlpha);
    mTimeMult = time_mult_was;
}

void BigAnimation::setAlpha(uint8_t alpha) noexcept {
    mAlpha = alpha;
    for (auto frame : mapTextures)
        SDL_SetTextureAlphaMod(frame, mAlpha);
}

void BigAnimation::childClean() {
    for (auto frame : mapTextures)
        SDL_DestroyTexture(frame);

    mapTextures.clear();
}
