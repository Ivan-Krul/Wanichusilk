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
// how tho?
//
// we got a dimensions of render window
// and we render within the region tiles
//
// |---------|
// |[0][5][1]|
// |[2][2][1]|
// |[3][3][2]|
// |---------|
//


class film::LayerTileset : public LayerRect {
public:
    LayerTileset(Clock* clock, ImageManager* imgmgr, LockerIndex imgind, uint16_t tileset_width, uint16_t tileset_height);
    void update() override;
    void render() const override;
    void clear() override;
    inline TimerStep getLongestWaiting() const noexcept override;

    virtual ~LayerTileset() = default;

private:
    inline void pushImgIndSetter(KeypointLayerInteractSwap* keypoint);
    inline void pushTSetResSetter(KeypointLayerInteractTilesetResize* keypoint);
    
    bool onPushSetter(KeypointLayer* keypoint) override;
    bool onPushTracker(const LockerIndex ease_indx) override;

    inline bool areAllTransitParamDefault() const noexcept { return mRect.is_default() && mColorAlpha.is_default(); }

    ImageManager* pImgMgr;
    Image* pImage;

    TransitParam<SDL_Color> mColorAlpha;
    
    // resolution of abstract map (4x4 texture map)
    uint16_t mTilesCountWidth = 0;
    uint16_t mTilesCountHeight = 0;
 
    // resolution of each tile (128x128 pixels)
    uint16_t mTileWidth = 0;
    uint16_t mTileHeight = 0;

    // resolution of viewing map (3x3 tiles)
    uint16_t mTileCountViewWidth = 0;
    uint16_t mTileCountViewHeight = 0;

    // tile indexes in grid order of view [y[x[]][x[]]]
    std::vector<uint8_t> maTileIndexes;

    ImageIndex mImgInd;

};
