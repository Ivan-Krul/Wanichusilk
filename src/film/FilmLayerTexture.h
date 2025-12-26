#pragma once
#include "FilmLayerRect.h"
#include "FilmKeypointLayer.h"
#include "../TextureManager.h"

namespace film {
    class LayerTexture;
}

class film::LayerTexture : public LayerRect {
public:
    LayerTexture(Clock* clock, TextureManager* texmgr, LockerIndex texind);
    void update() override;
    void render() const override;
    inline void clear() override;
    inline bool isWaiting() const noexcept override;
    inline TimerStep getLongestWaiting() const noexcept override;

    virtual ~LayerTexture() { clear(); }

private:
    inline void pushTexIndSetter(KeypointLayerInteractSwap* keypoint);

    bool onPushSetter(KeypointLayer* keypoint) override;
    bool onPushTracker(const LockerIndex ease_indx) override;

    inline void renderSwap(const SDL_FRect* res_rect, const SDL_FRect* res_part, uint8_t max_alpha) const;
    inline void finalizeSwap(LockerSimple<LayerBase::Tracker>::Iterator iter);

    inline bool areAllTransitParamDefault() const noexcept { return mPart.is_default() && mRect.is_default() && mAlpha.is_default() && mTexInd.is_default(); }

    TextureManager* pTexMgr;

    TransitParam<SDL_FRect> mPart;
    TransitParam<uint8_t> mAlpha;
    TransitParam<TextureIndex> mTexInd;

    Texture* pTexture;
};
