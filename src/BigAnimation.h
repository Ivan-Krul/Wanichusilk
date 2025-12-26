#pragma once
#include "Animation.h"

class BigAnimation : public Animation {
public:
    inline bool    isBig() const noexcept override { return true; }

    void start(float time_mult = 1.f) override;
    void render() override;
    void renderRaw(const SDL_FRect* rect, const uint8_t alpha = 255, const float time_mult = 1.f) override;

    void setAlpha(uint8_t alpha) noexcept override;

    ~BigAnimation() {
        for (auto frame : mapTextures)
            SDL_DestroyTexture(frame);
    }
private:
    void childClean() override;

    std::vector<SDL_Texture*> mapTextures;
};
