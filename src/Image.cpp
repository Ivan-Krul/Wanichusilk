#include "Image.h"
#include "Logger.h"

Image::Image(Image&& img) noexcept {
	mImage = img.mImage;
	mRectView = img.mRectView;
	mRectSnap = img.mRectSnap;
	mState = img.mState;
	
    img.mImage.tex = nullptr;
	img.mState.is_empty = true;
}

bool Image::createEmpty(int w, int h) {
	if(mState.is_preprocessed || !mState.is_empty) {
		Logger log(DEFAULT_LOG_PATH);
        log.logErrorIn(__FUNCTION__, "Image is not empty.");
        return true;
	}
	
	mImage.size.w = w;
	mImage.size.h = h;
	mState.is_empty = true;
	mState.color = {255, 255, 255, 255};
	return false;
}

bool Image::createFillin(int w, int h) {
	if(mState.is_preprocessed) {
		Logger log(DEFAULT_LOG_PATH);
        log.logErrorIn(__FUNCTION__, "Image is not empty.");
        return true;
	}
	
	mImage.surf = SDL_CreateSurface(w, h, cPixelFormat);
	if(!mImage.surf) {
		logSDLErr(__FUNCTION__);
		return true;
	}
	mState.is_empty = false;
	mState.color = {255, 255, 255, 255};
	return false;
}

bool Image::createLoad(const char* src) {
	if(mState.is_preprocessed || !mState.is_empty) {
		Logger log(DEFAULT_LOG_PATH);
        log.logErrorIn(__FUNCTION__, "Image is not empty.");
        return true;
	}
	
    SDL_Surface* surf = IMG_Load(src);
    if (!surf) {
		logSDLErr(__FUNCTION__);
        return true;
    }
	mImage.surf = SDL_ConvertSurface(surf, cPixelFormat);
	if (!mImage.surf) {
		logSDLErr(__FUNCTION__);
        return true;
    }
	SDL_DestroySurface(surf);
	
	mState.is_empty = false;
	mState.color = {255, 255, 255, 255};
	return false;
}

bool Image::preprocess(SDL_Renderer* renderer, SDL_TextureAccess access) {
	if(mState.is_empty) {
		if(mImage.size.w == 0 && mImage.size.w == mImage.size.h) {
			Logger log(DEFAULT_LOG_PATH);
			log.logErrorIn(__FUNCTION__, "Image for preprocessing is size 0");
			return true;
		}
		mRectView.w = mImage.size.w;
		mRectView.h = mImage.size.h;
		SDL_Texture* tex = SDL_CreateTexture(renderer, cPixelFormat, access, mImage.size.w, mImage.size.h);
		if(!tex) {
			logSDLErr(__FUNCTION__);
			return true;
		}
		mImage.tex = tex;
	}
	else {
		mRectView.w = mImage.surf->w;
		mRectView.h = mImage.surf->h;
		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, mImage.surf);
		if(!tex) {
			logSDLErr(__FUNCTION__);
			return true;
		}
		mImage.tex = tex;
	}
	if(!SDL_SetTextureScaleMode(mImage.tex, (SDL_ScaleMode)mState.scale_mode)) {
		logSDLErr(__FUNCTION__);
		return true;
	}
	
	mState.is_empty = false;
	mState.is_preprocessed = true;
	return false;
}

void Image::setColorAlpha(SDL_Color color) {
	if(mState.is_preprocessed) {
		mState.color = color;
		SDL_SetTextureColorMod(mImage.tex, color.r, color.g, color.b);
		SDL_SetTextureAlphaMod(mImage.tex, color.a);
	} else if(!mState.is_empty) {
		mState.color = color;
		SDL_SetSurfaceColorMod(mImage.surf, color.r, color.g, color.b);
		SDL_SetSurfaceAlphaMod(mImage.surf, color.a);
	} else {
		Logger log(DEFAULT_LOG_PATH);
		log.logErrorIn(__FUNCTION__, "Image is empty.");
	}
}

void Image::setScaleMode(SDL_ScaleMode mode) {
	if(mState.is_empty) {
		Logger log(DEFAULT_LOG_PATH);
		log.logErrorIn(__FUNCTION__, "Image is empty.");
		return;
	}
	mState.scale_mode = mode;
	if(!mState.is_preprocessed) {
		Logger log(DEFAULT_LOG_PATH);
		log.logWarningIn(__FUNCTION__, "Image got scale mode but did not apply.");
	}
	if (!SDL_SetTextureScaleMode(mImage.tex, mode)) 
		logSDLErr(__FUNCTION__);
}

void Image::setBlendMode(SDL_BlendMode mode) {
	if(mState.is_empty) {
		Logger log(DEFAULT_LOG_PATH);
		log.logErrorIn(__FUNCTION__, "Image is empty.");
		return;
	}
	mState.scale_mode = mode;
	if(mState.is_preprocessed) {
		if(!SDL_SetTextureBlendMode(mImage.tex, mode)) logSDLErr(__FUNCTION__);
	} else
		if (!SDL_SetSurfaceBlendMode(mImage.surf, mode)) logSDLErr(__FUNCTION__);		
}

SDL_Surface* Image::getSurface(const char* from_func) const {
	Logger log(DEFAULT_LOG_PATH);
	log.logInfoIn(__FUNCTION__, "Possibly unsafe call: %s.", from_func);
	if(mState.is_empty) {
		log.logError("^ Image is empty.");
		return nullptr;
	}
	if(mState.is_preprocessed) {
		log.logError("^ Image is in texture buffer.");
		return nullptr;
	}
	
	return mImage.surf;
}

SDL_Texture* Image::getTexture(const char* from_func) const {
	Logger log(DEFAULT_LOG_PATH);
	log.logInfoIn(__FUNCTION__, "Possibly unsafe call: %s.", from_func);
	if(mState.is_empty) {
		log.logError("^ Image is empty.");
		return nullptr;
	}
	if(!mState.is_preprocessed) {
		log.logError("^ Image is in surface buffer.");
		return nullptr;
	}
	
	return mImage.tex;
}

void Image::render() const {
    if (mState.is_preprocessed) SDL_RenderTexture(SDL_GetRendererFromTexture(mImage.tex), mImage.tex, mState.use_rectsnap ? &mRectSnap : NULL, &mRectView);
}

void Image::clear() {
	if(!mState.is_empty) {
		if(mState.is_preprocessed) SDL_DestroyTexture(mImage.tex);
		else SDL_DestroySurface(mImage.surf);
	}
	
	mState = { SDL_Color{255, 255, 255, 255}, SDL_BLENDMODE_NONE, true, false, false, SDL_SCALEMODE_NEAREST };
	SDL_FRect     mRectSnap = { 0.f };
    SDL_FRect     mRectView = { 0.f };
	
	mImage.surf = nullptr;
}

Image::~Image() {
	if(mState.is_empty) return;
	
	if(mState.is_preprocessed) {
	    if (mImage.tex)
			SDL_DestroyTexture(mImage.tex);
	}
	else
		if(mImage.surf)
			SDL_DestroySurface(mImage.surf);
		 
}
