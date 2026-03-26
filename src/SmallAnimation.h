#pragma once
#include "Animation.h"
#include "Image.h"

class SmallAnimation : public Animation {
public:
    using TexTileAmount = uint8_t;

public:
    using Animation::create;

    inline SmallAnimation() = default;
    SmallAnimation(Animation&& inst, char& is_fail);
    bool createLoad(const char* path) override;
	bool createConvert(size_t frames, float delay_ms, const Image& image);

    bool preprocess(SDL_Renderer* renderer) override;

    inline bool    isBig() const noexcept override { return false; }

    void render() override;
    void renderRaw(const SDL_FRect* rect, const uint8_t alpha = 255, const float time_mult = 1.f) override;

    void setAlpha(uint8_t alpha) noexcept override;
    void setScaleMode(SDL_ScaleMode mode) noexcept override;

    ~SmallAnimation() {
        if(mpTiles.tex) SDL_DestroyTexture(mpTiles.tex);
    }

private:
    void childClean() override;
    bool packAnimationInSingleSurface();
    inline void findTileResolution();

    PictureMap mpTiles;
    SDL_FRect mSrcRect = { 0.f };
    TexTileAmount mTileWidth = 0;
    TexTileAmount mTileHeight = 0;
};
