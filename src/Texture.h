#pragma once
#include "define.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
// needs rotation too

class Texture {
public:
    inline Texture() = default;
    inline Texture(const char* src, SDL_Renderer* renderer) { create(src, renderer); }
    bool   create(const char* src, SDL_Renderer* renderer);

    inline void setAlpha(uint8_t alpha) { mAlpha = alpha; SDL_SetTextureAlphaMod(mpTexture, mAlpha); }
    inline void setWidth(float w) { mRectRes.w = w; }
    inline void setHeight(float h) { mRectRes.h = h; }
    inline void setResolution(float w, float h) { mRectRes.w = w; mRectRes.h = h; }
    inline void setOffsetX(float x) { mRectRes.x = x; }
    inline void setOffsetY(float y) { mRectRes.y = y; }
    inline void setOffset(float x, float y) { mRectRes.x = x; mRectRes.y = y; }
    inline void setPartialRenderingResolution(float x = 0, float y = 0, float w = 0, float h = 0) { mRectPart.x = x; mRectPart.y = y; mRectPart.w = w; mRectPart.h = h; }
    inline void setPartialRenderingUsage(bool use) { mUseRectPart = use; }

    inline SDL_Texture*  getTexture()       noexcept { return mpTexture; }
    inline SDL_FRect     getRectPart() const noexcept { return mRectPart; }
    inline SDL_FRect     getRectRes()    const noexcept { return mRectRes; }

    void renderRaw(const SDL_FRect* src, const SDL_FRect* rect, const uint8_t alpha = 255) const;
    void render() const;

    void   clear();
    inline ~Texture() { clear(); }
protected:
    SDL_Texture* mpTexture = NULL;

    SDL_FRect     mRectPart = { 0.f };
    SDL_FRect     mRectRes = { 0.f };

    SDL_Renderer* mpRendererOrigin = NULL;

    uint8_t mAlpha = 255;
    bool mUseRectPart = false;

};
