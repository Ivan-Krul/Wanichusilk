#pragma once
#include <vector>

#include <SDL3_image/SDL_image.h>

#include "LockerSimple.h"
#include "Texture.h"

typedef int ResourceIndex;

class TextureManager {
public:
    inline void SetRenderer(SDL_Renderer* renderer) { mpRenderer = renderer; }
    inline SDL_Renderer* GetRenderer() const { return mpRenderer; }

    Texture& GetLockerTexture(ResourceIndex index) { assert(index != -1);  return mTextureLocker[index]; }
    ResourceIndex RequestTextureLoad(const char* path) {
        Texture tex;
        bool ret = tex.create(path, mpRenderer);
        if (!ret) return false;
        mTextureLocker.pushInLocker(std::move(tex));
        return true;
    }

    void RequestTextureClean(ResourceIndex index) {
        mTextureLocker.popFromLocker(index);
    }

private:
    LockerSimple<Texture> mTextureLocker;

    SDL_Renderer* mpRenderer = nullptr;
};
