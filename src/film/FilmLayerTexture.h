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
    void clear() override;
    inline TimerStep getLongestWaiting() const noexcept override;

    virtual ~LayerTexture() { clear(); }

private:
    inline void pushTexIndSetter(KeypointLayerInteractSwap* keypoint);

    bool onPushSetter(KeypointLayer* keypoint) override;
    bool onPushTracker(const LockerIndex ease_indx) override;

    inline bool areAllTransitParamDefault() const noexcept { return mPart.is_default() && mRect.is_default() && mAlpha.is_default(); }

    TextureManager* pTexMgr;
    Texture* pTexture;

    TransitParam<SDL_FRect> mPart;
    TransitParam<uint8_t> mAlpha;
    
    TextureIndex mTexInd;

};
