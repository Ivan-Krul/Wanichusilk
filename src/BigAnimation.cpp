#include "BigAnimation.h"
#include "Logger.h"

BigAnimation::BigAnimation(Animation&& inst, char& is_fail) {
    if (is_fail = create(std::move(inst))) return;

    is_fail = convertSurfaces();
}

bool BigAnimation::create(const char* path, SDL_Renderer* renderer) {
    if(baseCreate(path, renderer)) return true;

    if (convertSurfaces()) return true;
    return false;
}

void BigAnimation::preprocess() {
    SDL_Surface* surf;
    for (auto f = 0; f < mapTextures.size(); f++) {
        surf = mapTextures[f].surf;
        mapTextures[f].tex = SDL_CreateTextureFromSurface(mpRendererOrigin, surf);

        if (mapTextures[f].tex) {
            if (mAlpha != 255)
                SDL_SetTextureAlphaMod(mapTextures[f].tex, mAlpha);
        }
        else {
            Logger log(DEFAULT_LOG_SDL_PATH);
            log.logWarningIn(__FUNCTION__, "Convertion to texture was failed.");
            Logger log_sdl(DEFAULT_LOG_SDL_PATH);
            log_sdl.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        }
        SDL_DestroySurface(surf);
    }
    if(!mapTextures.empty()) mState.is_preprocessed = true;
}

void BigAnimation::render() {
    if (preRender()) return;
    if (mapTextures[mFrameIndex].tex) {
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "Texture wasn't initialised.");
        return;
    }

    SDL_RenderTexture(mpRendererOrigin, mapTextures[mFrameIndex].tex, nullptr, &mRect);
}

void BigAnimation::renderRaw(const SDL_FRect* rect, const uint8_t alpha, const float time_mult) {
    const float time_mult_was = mTimeMult;
    mTimeMult = time_mult;

    if (preRender()) return;
    if (!mapTextures[mFrameIndex].tex) {
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "Texture wasn't initialised.");
        return;
    }

    SDL_SetTextureAlphaMod(mapTextures[mFrameIndex].tex, alpha);

    SDL_RenderTexture(mpRendererOrigin, mapTextures[mFrameIndex].tex, nullptr, rect);

    SDL_SetTextureAlphaMod(mapTextures[mFrameIndex].tex, mAlpha);
    mTimeMult = time_mult_was;
}

void BigAnimation::setAlpha(uint8_t alpha) noexcept {
    mAlpha = alpha;
    for (auto frame : mapTextures)
        SDL_SetTextureAlphaMod(frame.tex, mAlpha);
}

void BigAnimation::childClean() {
    for (auto frame : mapTextures)
        if(frame.tex) SDL_DestroyTexture(frame.tex);

    mapTextures.clear();
}

bool BigAnimation::convertSurfaces() {
    if (!muHandle.anim || !mState.has_head) {
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "Animation won't start because of miss-match of heads.");
        if(!mState.has_head) log.logInfo("^ It mark as no head");
        if(!muHandle.anim) log.logInfo("^ It has no ptr");
        return true;
    }

    mapTextures.resize(mDelays_ms.size());
    for (auto f = 0; f < mapTextures.size(); f++) {
        mapTextures[f].surf = SDL_ConvertSurface(muHandle.anim->frames[f], cPixelFormat);
        if(!mapTextures[f].surf) {
            Logger log(DEFAULT_LOG_PATH);
            log.logWarningIn(__FUNCTION__, "Convertion surfaces was failed.");
            Logger log_sdl(DEFAULT_LOG_SDL_PATH);
            log_sdl.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
            return true;
        }
    }

    return false;
}

