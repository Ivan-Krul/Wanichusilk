#pragma once
#include <vector>

#include "IResourceManager.h"
#include "LockerSimple.h"
#include "Texture.h"

using TextureIndex = LockerIndex;

class TextureManager : public IResourceManager, public IResourceAccesser<Texture>, public IRendererGiver {
public:
    inline void SetRenderer(SDL_Renderer* renderer) noexcept override { mpRenderer = renderer; }
    inline SDL_Renderer* GetRenderer() const noexcept override { return mpRenderer; }
    
    LockerIndex RequestResourceCreate() override { return -1; };
    inline Texture* GetLockerResource(LockerIndex index) override { assert(index != -1);  return &mTextureLocker[index]; }
    LockerIndex RequestResourceLoad(const char* path) override {
        Texture tex;
        bool ret = tex.create(path, mpRenderer);
        if (!ret) return -1;
        return mTextureLocker.pushInLocker(std::move(tex));
    }
    
    inline void RequestResourceClean(LockerIndex index) override {
        mTextureLocker.popFromLocker(index);
    }

    inline Attribute GetAttribute() const noexcept override { return Attribute::RendererGiver | Attribute::Accesser; }

private:
    SDL_Renderer* mpRenderer = nullptr;
    LockerSimple<Texture> mTextureLocker;
};
