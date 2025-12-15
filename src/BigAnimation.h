#pragma once
#include "Animation.h"

class BigAnimation : public Animation {
public:
    inline bool    isBig() const noexcept override { return true; }

    void start(float time_mult = 1.f) override;
    void render() override;

    void setAlpha(uint8_t alpha) noexcept override;

private:
    std::vector<SDL_Texture*> mapTextures;
};
