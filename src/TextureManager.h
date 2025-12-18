#pragma once
#include <vector>

#include "LockerSimple.h"
#include "Texture.h"

using TextureIndex = LockerIndex;

class TextureManager {
public:
    inline void SetRenderer(SDL_Renderer* renderer) { mpRenderer = renderer; }
    inline SDL_Renderer* GetRenderer() const { return mpRenderer; }
    
    inline Texture& GetLockerTexture(TextureIndex index) { assert(index != -1);  return mTextureLocker[index]; }
    TextureIndex RequestTextureLoad(const char* path) {
        Texture tex;
        bool ret = tex.create(path, mpRenderer);
        if (!ret) return -1;
        return mTextureLocker.pushInLocker(std::move(tex));
    }
    
    inline void RequestTextureClean(TextureIndex index) {
        mTextureLocker.popFromLocker(index);
    }

private:
    SDL_Renderer* mpRenderer = nullptr;
    LockerSimple<Texture> mTextureLocker;
};
