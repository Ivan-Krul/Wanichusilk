#pragma once
#include "Animation.h"

class BigAnimation : public Animation {
public:
    using Animation::create;

    inline BigAnimation() = default;
    BigAnimation(Animation&& inst, char& is_fail);
    bool create(const char* path, SDL_Renderer* renderer) override;

    bool preprocess() override;

    inline bool    isBig() const noexcept override { return true; }

    void render() override;
    void renderRaw(const SDL_FRect* rect, const uint8_t alpha = 255, const float time_mult = 1.f) override;

    void setAlpha(uint8_t alpha) noexcept override;

    ~BigAnimation() {
        for (auto frame : mapTextures)
            if(frame.tex) SDL_DestroyTexture(frame.tex);
    }
private:
    void childClean() override;
    bool convertSurfaces();

    std::vector<PictureMap> mapTextures;


};
