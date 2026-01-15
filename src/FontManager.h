#pragma once
#include <SDL3_ttf/SDL_ttf.h>

#include "IResourceManager.h"
#include "Logger.h"

class FontManager : public IResourceManager, public IResourceAccesser<TTF_Font> {
public:
    struct LoadParamConvertor : public IResourceLoadParamConvertor {
        const char* path;
        float size = 10.f;
        inline ResourceLoadParams to_param() const noexcept override { return ResourceLoadParams{ path, *reinterpret_cast<const size_t*>(&size) }; }
    };

public:
    LockerIndex RequestResourceCreate() override { return -1; };
    inline TTF_Font* GetLockerResource(LockerIndex index) override { assert(index != -1); return mFontLocker[index]; }
    LockerIndex RequestResourceLoad(ResourceLoadParams load) override {
        TTF_Font* font = TTF_OpenFont(load.path, *reinterpret_cast<float*>(&load.extra));
        if (font == nullptr) {
            Logger log(DEFAULT_LOG_SDL_PATH);
            log.logErrorIn(__FUNCTION__, "%s.", SDL_GetError());
            return -1;
        }
        return mFontLocker.pushInLocker(font);
    }

    inline void RequestResourceClean(LockerIndex index) override {
        TTF_CloseFont(mFontLocker[index]);
        mFontLocker.popFromLocker(index);
    }

    inline Attribute GetAttribute() const noexcept override { return Attribute::Accesser; }

    inline ~FontManager() {
        for (auto font : mFontLocker) {
            TTF_CloseFont(font);
        }
    }
private:
    LockerSimple<TTF_Font*> mFontLocker;
};


