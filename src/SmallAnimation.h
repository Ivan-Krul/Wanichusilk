#pragma once
#include "Animation.h"

class SmallAnimation : public Animation {
public:
    bool create(Animation&& instance);

    inline virtual bool    isBig()                const noexcept { return false; }

    void start(float time_mult = 1.f) override;
    void render() override;

private:
    bool packAnimationInRendTexture();
    inline void findTileResolution();

    SDL_Texture* mpRenderTextureTile = nullptr;
    SDL_FRect mRenderTexTileRect;
    char mRenderTexTileWidth;
    char mRenderTexTileHeight;
};
