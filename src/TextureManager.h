#pragma once
#include <vector>

#include <SDL3_image/SDL_image.h>

#include "Locker.h"
#include "Texture.h"

typedef int ResourceIndex;

class TextureManager {
    struct TextureArgContainer {
        const char* path;
        RENDERER* renderer;
    };

public:
    TextureManager() : mTextureLocker(
        [](Texture& tex, TextureArgContainer cont) -> bool { return tex.create(cont.path, cont.renderer);  }) {
    }

    inline void SetRenderer(RENDERER* renderer) { mpRenderer = renderer; }
    inline RENDERER* GetRenderer() const { return mpRenderer; }

    Texture& GetLockerTexture(ResourceIndex index) { return mTextureLocker[index]; }
    ResourceIndex RequestTextureLoad(const char* path) {
        TextureArgContainer cont;
        cont.path = path;
        cont.renderer = mpRenderer;
        return mTextureLocker.pushInLocker(cont);
    }

    void RequestTextureClean(ResourceIndex index) {
        mTextureLocker.popFromLocker(index);
    }

private:
    
    Locker<Texture, TextureArgContainer> mTextureLocker;

    RENDERER* mpRenderer = nullptr;
};
