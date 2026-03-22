#pragma once
#include "IResourceManager.h"
#include "Texture.h"

using TextureIndex = LockerIndex;

// so instead of refitting texture every time for certain operation, you should clone it instead

class TextureManager : public IResourceManager, public IResourceAccesser<Texture>, public IRendererGiver {
public:
    struct LoadParamConvertor : public IResourceLoadParamConvertor {
        const char* path;
        SDL_ScaleMode scalemode = SDL_SCALEMODE_NEAREST;
        inline ResourceLoadParams to_param() const noexcept override { return ResourceLoadParams{ path, (size_t)scalemode }; }
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
		tex.setScaleMode(*reinterpret_cast<SDL_ScaleMode*>(&load.extra));
		return mTextureLocker.pushInLocker(std::move(tex));
    }
    
    void RequestResourceClean(LockerIndex index) override {
        mTextureLocker.popFromLocker(index);
    }

    inline Attribute GetAttribute() const noexcept override { return Attribute::RendererGiver | Attribute::Accesser; }
    
private:
    SDL_Renderer* mpRenderer = nullptr;
    Locker<Texture> mTextureLocker;
};
