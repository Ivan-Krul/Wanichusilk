#include "Texture.h"
#include "Logger.h"

Texture::Texture(Texture&& tex) noexcept : 
							mpTexture(tex.mpTexture),
							mRectRes(tex.mRectRes),
							mRectPart(tex.mRectPart),
							mpRendererOrigin(tex.mpRendererOrigin),
							mState(tex.mState)
							{
    tex.mpTexture = nullptr;
}

bool Texture::create(const char* src, SDL_Renderer* renderer) {
    if (mpTexture) return false;
    mpRendererOrigin = renderer;

    mpTexture = IMG_LoadTexture(mpRendererOrigin, src);
    if (mpTexture == NULL) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
        return false;
    }
    mRectRes.w = mpTexture->w;
    mRectRes.h = mpTexture->h;

	mState.color = {255,255,255,255};
    mState.has_alpha = SDL_ISPIXELFORMAT_ALPHA(mpTexture->format);

    return true;
}

bool Texture::create(SDL_Texture* tex) {
    if (mpTexture || !tex) return false;
    mpRendererOrigin = SDL_GetRendererFromTexture(tex);

    mpTexture = tex;
    mRectRes.w = mpTexture->w;
    mRectRes.h = mpTexture->h;

	mState.color = {255,255,255,255};
    mState.has_alpha = SDL_ISPIXELFORMAT_ALPHA(mpTexture->format);

    return true;
}

void Texture::setColorAlpha(SDL_Color color) {
	if(!mpTexture) return;
	mState.color = color;
	SDL_SetTextureColorMod(mpTexture, color.r, color.g, color.b);
	
	if (mState.has_alpha) {
		SDL_SetTextureAlphaMod(mpTexture, color.a);
	} else {
		Logger log(DEFAULT_LOG_PATH);
		log.logWarningIn(__FUNCTION__, "the texture doesn't have alpha channel.");
	}
}

void Texture::setScaleMode(SDL_ScaleMode mode) {
	if(!mpTexture) return;
	if (!SDL_SetTextureScaleMode(mpTexture, mode)) 
		logSDLErr(__FUNCTION__);
	
}

void Texture::renderRaw(const SDL_FRect* src, const SDL_FRect* rect, const uint8_t alpha) const {
    if (mState.has_alpha) {
        SDL_SetTextureAlphaMod(mpTexture, alpha);
        SDL_RenderTexture(mpRendererOrigin, mpTexture, src, rect);
        SDL_SetTextureAlphaMod(mpTexture, mState.color.a);
    }
    else 
        SDL_RenderTexture(mpRendererOrigin, mpTexture, src, rect);
}

void Texture::render() const {
    if (mpTexture)
        SDL_RenderTexture(mpRendererOrigin, mpTexture, mState.use_rectpart ? &mRectPart : NULL, &mRectRes);
}

void Texture::clear() {
    if(mpTexture)
        SDL_DestroyTexture(mpTexture);

    mpTexture = NULL;
}
