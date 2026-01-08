#pragma once
#include <SDL3_ttf/SDL_ttf.h>

#include "IResourceManager.h"

class FontManager : public IResourceManager, public IResourceAccesser<void> {
public:
    LockerIndex RequestResourceCreate() override { return -1; };
    inline void* GetLockerResource(LockerIndex index) override { assert(index != -1);  return nullptr; }// &mTextureLocker[index]; }
    LockerIndex RequestResourceLoad(const char* path) override {
        //auto font = SDL_FONT
        //bool ret = tex.create(path, mpRenderer);
        //if (!ret) return -1;
        //return mTextureLocker.pushInLocker(std::move(tex));
        return -1;
    }

    inline void RequestResourceClean(LockerIndex index) override {
        //mTextureLocker.popFromLocker(index);
    }

    inline Attribute GetAttribute() const noexcept override { return Attribute::Accesser; }

};


