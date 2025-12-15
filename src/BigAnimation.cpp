#include "BigAnimation.h"

void BigAnimation::start(float time_mult) {
    if (!mpAnimation) return;
    mFrameIndex = 0;
    mTimeMult = time_mult;

    SDL_Surface* surf;

    mapTextures.resize(mDelays_ms.size());
    for (auto f = 0; f < mapTextures.size(); f++) {
        surf = SDL_ConvertSurface(mpAnimation->frames[f], cPixelFormat);
        mapTextures[f] = SDL_CreateTextureFromSurface(mpRendererOrigin, surf);

        if (mapTextures[f] && mAlpha != 255)
            SDL_SetTextureAlphaMod(mapTextures[f], mAlpha);

        SDL_DestroySurface(surf);
    }

    mCurrentDelay = std::chrono::milliseconds(mDelays_ms[0]);
}

void BigAnimation::render() {
    if (preRender()) return;

    SDL_RenderTexture(mpRendererOrigin, mapTextures[mFrameIndex], nullptr, &mRect);
}

void BigAnimation::setAlpha(uint8_t alpha) noexcept {
    mAlpha = alpha;
    for (auto frames : mapTextures)
        SDL_SetTextureAlphaMod(frames, mAlpha);
}
