#include "Texture.h"

Texture::Texture(Texture&& tex) noexcept {
    mpTexture = tex.mpTexture;

    mRectPart = tex.mRectPart;
    mRectRes = tex.mRectRes;
    mpRendererOrigin = tex.mpRendererOrigin;
    mAlpha = tex.mAlpha;
    mUseRectPart = tex.mUseRectPart;
    mHasAlpha = tex.mHasAlpha;

    tex.mpTexture = nullptr;
}

bool Texture::create(const char* src, SDL_Renderer* renderer) {
    if (mpTexture) return false;
    mpRendererOrigin = renderer;

    mpTexture = IMG_LoadTexture(mpRendererOrigin, src);
    if (mpTexture == NULL) return false;
    mRectRes.w = mpTexture->w;
    mRectRes.h = mpTexture->h;

    mHasAlpha = SDL_ISPIXELFORMAT_ALPHA(mpTexture->format);

    return true;
}

void Texture::renderRaw(const SDL_FRect* src, const SDL_FRect* rect, const uint8_t alpha) const {
    if (mHasAlpha) {
        SDL_SetTextureAlphaMod(mpTexture, alpha);
        SDL_RenderTexture(mpRendererOrigin, mpTexture, src, rect);
        SDL_SetTextureAlphaMod(mpTexture, mAlpha);
    }
    else SDL_RenderTexture(mpRendererOrigin, mpTexture, src, rect);
}

void Texture::render() const {
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
