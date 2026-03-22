#pragma once
#include "define.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "Logger.h"

class Image {
public:
    inline Image() = default;
    Image(Image&& tex) noexcept;
	bool   createEmpty(int w, int h);
	bool   createFillin(int w, int h);
    bool   createLoad(const char* src);
	
    bool   preprocess(SDL_Renderer* renderer, SDL_TextureAccessMode access);
	
	       void setColorAlpha(SDL_Color color);
    inline void setWidth(float w) { mRectRes.w = w; }
    inline void setHeight(float h) { mRectRes.h = h; }
    inline void setResolution(float w, float h) { mRectRes.w = w; mRectRes.h = h; }
    inline void setOffsetX(float x) { mRectRes.x = x; }
    inline void setOffsetY(float y) { mRectRes.y = y; }
    inline void setOffset(float x, float y) { mRectRes.x = x; mRectRes.y = y; }
    inline void setPartialRenderingResolution(float x = 0, float y = 0, float w = 0, float h = 0) { mRectPart.x = x; mRectPart.y = y; mRectPart.w = w; mRectPart.h = h; }
    inline void setPartialRenderingUsage(bool use) { mState.use_rectpart = use; }
	       void setScaleMode(SDL_ScaleMode mode);

    inline SDL_FRect     getRectPart() const noexcept { return mRectPart; }
    inline SDL_FRect     getRectRes()  const noexcept { return mRectRes;  }
	
    void render() const;

    void   clear();
    ~Texture();
protected:
	union PictureMap {
        SDL_Surface* surf = nullptr;
        SDL_Texture* tex;
		struct { int32_t w, int32_t h } size;
    } mImage;

    SDL_FRect     mRectPart = { 0.f };
    SDL_FRect     mRectRes = { 0.f };

	struct {
		SDL_Color color; // rgba
		uint8_t blendmode;
		uint8_t is_empty : 1;
		uint8_t is_preprocessed : 1
		uint8_t use_rectpart : 1;
	} mState = { SDL_Color{255, 255, 255, 255}, SDL_BLENDMODE_NONE, true, false, false };
};
