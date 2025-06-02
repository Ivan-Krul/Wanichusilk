#include "Texture.h"

bool Texture::create(const char* src, SDL_Renderer* renderer) {
    if (mpTexture) return false;
    mpRendererOrigin = renderer;

    mpTexture = IMG_LoadTexture(mpRendererOrigin, src);
    if (mpTexture == NULL) return false;
    mRect.w = mpTexture->w;
    mRect.h = mpTexture->h;

    return true;
}

void Texture::render() {
    if (mpTexture)
        SDL_RenderTexture(mpRendererOrigin, mpTexture, NULL, &mRect);
    else
        SDL_Log("Not rendering");
}

void Texture::clear() {
    if(mpTexture)
        SDL_DestroyTexture(mpTexture);

    mpTexture = NULL;
}
