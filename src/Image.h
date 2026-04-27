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
	
    bool   preprocess(SDL_Renderer* renderer, SDL_TextureAccess access = SDL_TEXTUREACCESS_STATIC);
	
	void          setColorAlpha(SDL_Color color);
	SDL_Color     getColorAlpha() const { return mState.color; }
		   
    void          setScaleMode(SDL_ScaleMode mode);
    SDL_ScaleMode getScaleMode() const { return (mState.scale_mode == 0b11) ? SDL_SCALEMODE_INVALID : (SDL_ScaleMode)mState.scale_mode; }

    void          setBlendMode(SDL_BlendMode mode);
    SDL_BlendMode getBlendMode() const { return (mState.blendmode == 0b11111111) ? SDL_BLENDMODE_INVALID : (SDL_BlendMode)mState.blendmode; }

	inline void      turnOffSnap() { mState.use_rectsnap = false; }
	inline void      setRectSnap(SDL_FRect snap) { mState.use_rectsnap = true; mRectSnap = snap; }
    inline SDL_FRect getRectSnap() const { return mRectSnap; }
	
	inline void      turnOffView() { mState.use_rectview = false; }
	inline void      setRectViewOffset(float x, float y) { mState.use_rectsnap = true; mRectView.x = x; mRectView.y = y; }
	inline void      setRectView(SDL_FRect view) { mState.use_rectsnap = true; mRectView = view; }
    inline SDL_FRect getRectView() const { return mRectView; }
	
    inline int getImageWidth() const noexcept { return (!mState.is_empty) ? ((mState.is_preprocessed) ? mImage.tex->w : mImage.surf->w) : mImage.size.w; }
    inline int getImageHeight() const noexcept { return (!mState.is_empty) ? ((mState.is_preprocessed) ? mImage.tex->h : mImage.surf->h) : mImage.size.h; }

	inline bool isEmpty() const { return mState.is_empty; }
	
	// function name for tracking in case of undefined behaviours
	SDL_Surface* getSurface(const char* from_func) const;
	SDL_Texture* getTexture(const char* from_func) const;
	
    void render() const; // render from texture buffer ONLY, let outer classes use surface how they intend

    void   clear();
    virtual ~Image();
public:
	static const SDL_PixelFormat cPixelFormat = SDL_PIXELFORMAT_RGBA32;
	
protected:
	union PictureMap {
        SDL_Surface* surf = nullptr;
        SDL_Texture* tex;
		struct { int32_t w, h; } size;
    } mImage;

    SDL_FRect     mRectSnap = { 0.f };
    SDL_FRect     mRectView = { 0.f };

	struct {
		SDL_Color color; // rgba
		uint8_t blendmode;
		uint8_t is_empty : 1;
		uint8_t is_preprocessed : 1;
		uint8_t use_rectsnap : 1;
        uint8_t use_rectview : 1;
		uint8_t scale_mode : 2;
	} mState = { SDL_Color{255, 255, 255, 255}, SDL_BLENDMODE_NONE, true, false, false, false, SDL_SCALEMODE_NEAREST };
};
