#pragma once
#include <vector>

#include "IResourceManager.h"
#include "Texture.h"

using TextureIndex = LockerIndex;

class TextureManager : public IResourceManager, public IResourceAccesser<Texture>, public IRendererGiver {
public:
    struct LoadParamConvertor : public IResourceLoadParamConvertor {
        const char* path;
        inline ResourceLoadParams to_param() const noexcept override { return ResourceLoadParams{ path, 0 }; }
    };

public:
    inline void SetRenderer(SDL_Renderer* renderer) noexcept override { mpRenderer = renderer; }
    inline SDL_Renderer* GetRenderer() const noexcept override { return mpRenderer; }
    
    LockerIndex RequestResourceCreate() override { return -1; };
    inline Texture* GetLockerResource(LockerIndex index) override { assert(index != -1);  return &mTextureLocker[index]; }
    LockerIndex RequestResourceLoad(ResourceLoadParams load) override {
        Texture tex;
        bool ret = tex.create(load.path, mpRenderer);
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
