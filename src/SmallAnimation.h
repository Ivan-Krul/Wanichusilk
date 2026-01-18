#pragma once
#include "Animation.h"

class SmallAnimation : public Animation {
public:
    using TexTileAmount = uint8_t;

public:
    inline SmallAnimation() = default;
    inline SmallAnimation(Animation&& inst) { create(std::move(inst)); }

    inline bool    isBig() const noexcept override { return false; }

    void start(float time_mult = 1.f) override;
    void render() override;
    void renderRaw(const SDL_FRect* rect, const uint8_t alpha = 255, const float time_mult = 1.f) override;

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
    TexTileAmount mRenderTexTileWidth = 0;
    TexTileAmount mRenderTexTileHeight = 0;
};
