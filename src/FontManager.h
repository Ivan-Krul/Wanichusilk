#pragma once
#include <SDL3_ttf/SDL_ttf.h>

#include "IResourceManager.h"
#include "Font.h"
#include "Logger.h"

using FontIndex = LockerIndex;

class FontManager : public IResourceManager, public IResourceAccesser<Font> {
public:
    struct LoadParamConvertor : public IResourceLoadParamConvertor {
        const char* path;
        float size = 10.f;
        inline ResourceLoadParams to_param() const noexcept override { return ResourceLoadParams{ path, *reinterpret_cast<const size_t*>(&size) }; }
    };

public:
    FontIndex RequestResourceCreate() override { return -1; };
    inline Font* GetLockerResource(FontIndex index) override { assert(index != -1); return &(mFontLocker[index]); }
    FontIndex RequestResourceLoad(ResourceLoadParams load) override {
        Font font;
        if (font.create(load.path, *reinterpret_cast<float*>(&load.extra))) {
            logSDLErr(__FUNCTION__);
            return -1;
        }
        return mFontLocker.pushInLocker(std::move(font));
    }

    inline void RequestResourceClean(FontIndex index) override {
        mFontLocker.popFromLocker(index);
    }

    inline Attribute GetAttribute() const noexcept override { return Attribute::Accesser; }
private:
    Locker<Font> mFontLocker;
};


