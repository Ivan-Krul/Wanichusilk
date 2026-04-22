#include "FilmLayerTileset.h"

film::LayerTileset::LayerTileset(Clock* clock, ImageManager* imgmgr, LockerIndex imgind, uint16_t tileset_width, uint16_t tileset_height) {
    setClock(clock);

    pImgMgr = imgmgr;
    mImgInd = imgind;

    pImage = imgind != -1 ? pImgMgr->GetLockerResource(imgind) : nullptr;

    mTilesCountWidth = tileset_width;
    mTilesCountHeight = tileset_height;
    
    if(pImage) {
        mTileWidth = (pImage) ? pImage->getImageWidth() / mTilesCountWidth : 0;
        mTileHeight = (pImage) ? pImage->getImageHeight() / mTilesCountHeight : 0;

        maTileIndexes.resize(mTilesCountWidth * mTilesCountHeight);

        for(size_t i = 0; i < maTileIndexes.size(); i++) maTileIndexes[i] = 0;
    }

    mRect.elem_to = pImage ? pImage->getRectView() : SDL_FRect{ 0.f };
    mColorAlpha.elem_to = { 255, 255, 255, 255 };

    mRect.ease_tracker.setClock(clock);
    mColorAlpha.ease_tracker.setClock(clock);

    mRect.reset_tracker();
}

void film::LayerTileset::update() {
    if (maEases.isEmpty()) return;
    mColorAlpha.ease_tracker.update();

    removePassedEases();
}

void film::LayerTileset::render() const {
	if (mImgInd == -1) return;
	
    SDL_FRect view = { 0.f };
    const auto res_view = computeRectRender(mRect, &view);
	//if(res_view) pImage->setRectView(*res_view);
    

    for(size_t i = 0; i < maTileIndexes.size(); i++) {
        pImage->setRectSnap({
            (maTileIndexes[i] % mTilesCountWidth) * mTileWidth,
            (maTileIndexes[i] / mTilesCountWidth) * mTileHeight,
            mTileWidth,
            mTileHeight});

        if (mColorAlpha.is_progress()) {
	        SDL_Color ca;
            ca.r = lerp(mColorAlpha.ease_tracker, mColorAlpha.elem_from.r, mColorAlpha.elem_to.r);
	        ca.g = lerp(mColorAlpha.ease_tracker, mColorAlpha.elem_from.g, mColorAlpha.elem_to.g);
	        ca.b = lerp(mColorAlpha.ease_tracker, mColorAlpha.elem_from.b, mColorAlpha.elem_to.b);
	        ca.a = lerp(mColorAlpha.ease_tracker, mColorAlpha.elem_from.a, mColorAlpha.elem_to.a);
	        pImage->setColorAlpha(ca);
        } else pImage->setColorAlpha(mColorAlpha.elem_to);

        pImage->render();
    }
}

void film::LayerTileset::clear() {
    maEases.clear();
    mRect.clear();
    mColorAlpha.clear();
    maTileIndexes.clear();
    mTileWidth = mTileHeight = 0;
    mTilesCountWidth = mTilesCountHeight = 0;
    mImgInd = -1;

    pImage = nullptr;
}

inline TimerStep film::LayerImage::getLongestWaiting() const noexcept {
    return clockfunc::max(mColorAlpha.ease_tracker.getLimiter(), mRect.ease_tracker.getLimiter());
}


