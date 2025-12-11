#pragma once
#include "Animation.h"

class SmallAnimation : public Animation {
public:
    //bool create(const char* path, SDL_Renderer* renderer);
    using Animation::create;

    bool create(Animation&& instance);

    inline bool    isBig() const noexcept override { return false; }

    void start(float time_mult = 1.f) override;
    void render() override;

    void setAlpha(uint8_t alpha) noexcept override;

private:
    bool packAnimationInRendTexture();
    inline void findTileResolution();

    SDL_Texture* mpRenderTextureTile = nullptr;
    char mRenderTexTileWidth;
    char mRenderTexTileHeight;
};
