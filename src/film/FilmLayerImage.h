#pragma once
#include "FilmLayerRect.h"
#include "FilmKeypointLayer.h"
#include "../ImageManager.h"

namespace film {
    class LayerImage;
}

// we deal with preprocessed image

class film::LayerImage : public LayerRect {
public:
    LayerImage(Clock* clock, ImageManager* imgmgr, LockerIndex texind);
    void update() override;
    void render() const override;
    void clear() override;
    inline TimerStep getLongestWaiting() const noexcept override;

    virtual ~LayerImage() = default;

private:
    inline void pushImgIndSetter(KeypointLayerInteractSwap* keypoint);

    bool onPushSetter(KeypointLayer* keypoint) override;
    bool onPushTracker(const LockerIndex ease_indx) override;

    inline bool areAllTransitParamDefault() const noexcept { return mSnap.is_default() && mRect.is_default() && mColorAlpha.is_default(); }

    ImageManager* pImgMgr;
    Image* pImage;

    TransitParam<SDL_FRect> mSnap;
    TransitParam<SDL_Color> mColorAlpha;
    
    ImageIndex mImgInd;

};
