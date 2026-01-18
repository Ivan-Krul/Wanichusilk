#pragma once
#include "Animation.h"

class BigAnimation : public Animation {
public:
    inline BigAnimation() = default;
    inline BigAnimation(Animation&& inst);
    bool create(const char* path, SDL_Renderer* renderer) override;

    void preprocess() override;

    inline bool    isBig() const noexcept override { return true; }

    void start(float time_mult = 1.f) override;
    void render() override;
    void renderRaw(const SDL_FRect* rect, const uint8_t alpha = 255, const float time_mult = 1.f) override;

    void setAlpha(uint8_t alpha) noexcept override;

    ~BigAnimation() {
        for (auto frame : mapTextures)
            if(frame.tex) SDL_DestroyTexture(frame);
    }
private:
    void childClean() override;
    void convertSurfaces();

    std::vector<PictureMap> mapTextures;


};
