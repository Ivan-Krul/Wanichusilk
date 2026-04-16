#pragma once
#include <stdint.h>
#include <vector>
#include "FilmLayerRect.h"
#include "FilmKeypointLayer.h"
#include "../ImageManager.h"

namespace film {
    class LayerTileset;
}

// we deal with preprocessed image

class film::LayerTileset : public LayerRect {
public:
    LayerTileset(Clock* clock, ImageManager* imgmgr, LockerIndex imgind, uint16_t tileset_width, uint16_t tileset_height);
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

    TransitParam<SDL_Color> mColorAlpha;
    
    uint16_t mTilesCountWidth = 0;
    uint16_t mTilesCountHeight = 0;

    uint16_t mTileWidth = 0;
    uint16_t mTileHeight = 0;
    
    std::vector<uint8_t> maTileIndexes;

    ImageIndex mImgInd;

};
