#include "Texture.h"

bool Texture::create(const char* src, SDL_Renderer* renderer) {
    if (mpTexture) return false;
    mpRendererOrigin = renderer;

    mpTexture = IMG_LoadTexture(mpRendererOrigin, src);
    if (mpTexture == NULL) return false;
    mRectRes.w = mpTexture->w;
    mRectRes.h = mpTexture->h;

    return true;
}

void Texture::renderRaw(SDL_FRect* src, SDL_FRect* rect, uint8_t alpha) {
    SDL_SetTextureAlphaMod(mpTexture, alpha);
    SDL_RenderTexture(mpRendererOrigin, mpTexture, src, rect);
    SDL_SetTextureAlphaMod(mpTexture, mAlpha);
}

void Texture::render() {
    if (mpTexture)
        SDL_RenderTexture(mpRendererOrigin, mpTexture, mUseRectPart ? &mRectPart : NULL, &mRectRes);
    else
        SDL_Log("Not rendering");
}

void Texture::clear() {
    if(mpTexture)
        SDL_DestroyTexture(mpTexture);

    mpTexture = NULL;
}
