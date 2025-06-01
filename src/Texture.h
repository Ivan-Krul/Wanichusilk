#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Texture {
public:
    inline Texture() = default;
    inline Texture(const char* src, SDL_Renderer* renderer) { create(src, renderer); }
    bool   create(const char* src, SDL_Renderer* renderer);

    inline void setWidth(float w) { mRect.w = w; }
    inline void setHeight(float h) { mRect.h = h; }
    inline void setResolution(float w, float h) { mRect.w = w; mRect.h = h; }
    inline void setOffsetX(float x) { mRect.x = x; }
    inline void setOffsetY(float y) { mRect.y = y; }
    inline void setOffset(float x, float y) { mRect.x = x; mRect.y = y; }

    inline SDL_Texture* getTexture()        noexcept { return mpTexture; }
    inline SDL_FRect     getRectSrc() const noexcept { return mRectSrc; }
    inline SDL_FRect     getRect()   const noexcept { return mRect; }

    void render();

    void   clear();
    inline ~Texture() { clear(); }
private:
    SDL_Texture* mpTexture = NULL;
    SDL_FRect     mRectSrc = { 0.f };
    SDL_FRect     mRect = { 0.f };

    SDL_Renderer* mpRendererOrigin = NULL;

};
