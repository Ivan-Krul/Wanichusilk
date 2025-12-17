#pragma once
#include "Animation.h"

class SmallAnimation : public Animation {
public:
    inline bool    isBig() const noexcept override { return false; }

    void start(float time_mult = 1.f) override;
    void render() override;

    void setAlpha(uint8_t alpha) noexcept override;

    ~SmallAnimation() {
        SDL_DestroyTexture(mpRenderTextureTile);
    }

private:
    void childClean() override;
    bool packAnimationInRendTexture();
    inline void findTileResolution();

    SDL_Texture* mpRenderTextureTile = nullptr;
    SDL_FRect mSrcRect = { 0.f };
    char mRenderTexTileWidth;
    char mRenderTexTileHeight;
};
