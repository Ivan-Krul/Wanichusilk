#include "FilmLayerTileset.h"

film::LayerTileset::LayerTileset(Clock* clock, ImageManager* imgmgr, LockerIndex imgind, uint16_t tileset_width, uint16_t tileset_height) {
    setClock(clock);

    pImgMgr = imgmgr;
    mImgInd = texind;

    pImage = imgind != -1 ? pImgMgr->GetLockerResource(imgind) : nullptr;

    mTilesCountWidth = tileset_width;
    mTilesCountHeight = tileset_height;
    
    if(pImage) {
        mTileWidth = (pImage) ? pImage->GetImageWidth() / mTilesCountWidth : 0;
        mTileHeight = (pImage) ? pImage->GetImageHeight() / mTilesCountHeight : 0;

        maTileIndexes.resize(mTilesCountWidth * mTilesCountHeight);

        for(size_t i = 0; i < maTilesIndexes.size(); i++) maTilesIndexes[i] = 0;
    }

    mRect.elem_to = pImage ? pImage->getRectView() : SDL_FRect{ 0.f };
    mColorAlpha.elem_to = { 255, 255, 255, 255 };

    mRect.ease_tracker.setClock(clock);
    mColorAlpha.ease_tracker.setClock(clock);

    mRect.reset_tracker();
}
