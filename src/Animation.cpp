#include "Animation.h"

bool Animation::create(const char* path) {
    if (mpAnimation) return false;
    mpRendererOrigin = renderer;
    mpAnimation = IMG_LoadAnimation(path);
    if (mpAnimation == NULL) return false;

    mRect.w = mpAnimation->w;
    mRect.h = mpAnimation->h;

    mDelays_ms.resize(mpAnimation->count);
    for (int i = 0; i < mpAnimation->count; i++) {
        mDelays_ms[i] = mpAnimation->delays[i];
    }

    return mpAnimation;
    //SDL_CreateSurface(mpAnimation->w, mpAnimation->h, SDL_PIXELFORMAT_RGBA32);
}

void Animation::start() {
    if (!mpAnimation) return;
    mFrameIndex = 0;
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

