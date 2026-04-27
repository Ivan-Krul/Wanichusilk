#include "FilmLayerTileset.h"

film::LayerTileset::LayerTileset(Clock* clock, ImageManager* imgmgr, LockerIndex imgind, uint16_t tileset_width, uint16_t tileset_height) {
    setClock(clock);

    pImgMgr = imgmgr;
    mImgInd = imgind;

    pImage = imgind != -1 ? pImgMgr->GetLockerResource(imgind) : nullptr;

    mTilesCountWidth = tileset_width;
    mTilesCountHeight = tileset_height;
    
    if(pImage) {
        mTileWidth = pImage->getImageWidth() / mTilesCountWidth;
        mTileHeight = pImage->getImageHeight() / mTilesCountHeight;

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
	
    if (mRect.is_default()) pImage->turnOffView();
    else {
        if (mRect.is_progress())
            pImage->setRectView(lerp_rect(mRect.elem_from, mRect.elem_to, mRect.ease_tracker));
        else pImage->setRectView(mRect.elem_to);
    }
    
    if (mColorAlpha.is_progress()) {
        SDL_Color ca;
        ca.r = lerp(mColorAlpha.ease_tracker, mColorAlpha.elem_from.r, mColorAlpha.elem_to.r);
        ca.g = lerp(mColorAlpha.ease_tracker, mColorAlpha.elem_from.g, mColorAlpha.elem_to.g);
        ca.b = lerp(mColorAlpha.ease_tracker, mColorAlpha.elem_from.b, mColorAlpha.elem_to.b);
        ca.a = lerp(mColorAlpha.ease_tracker, mColorAlpha.elem_from.a, mColorAlpha.elem_to.a);
        pImage->setColorAlpha(ca);
    } else pImage->setColorAlpha(mColorAlpha.elem_to);

    for(size_t i = 0; i < maTileIndexes.size(); i++) {
        pImage->setRectSnap({
            (float)((int16_t)(maTileIndexes[i] % mTilesCountWidth) * mTileWidth),
            (float)((int16_t)(maTileIndexes[i] / mTilesCountWidth) * mTileHeight),
            (float)mTileWidth,
            (float)mTileHeight});

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

inline TimerStep film::LayerTileset::getLongestWaiting() const noexcept {
    return clockfunc::max(mColorAlpha.ease_tracker.getLimiter(), mRect.ease_tracker.getLimiter());
}

inline void film::LayerTileset::pushImgIndSetter(KeypointLayerInteractSwap* keypoint) {
    KeypointLayerInteractSwap::SwapMode swapmode = keypoint->swap;

    mRect.shift_elem();

    mImgInd = keypoint->indx;
    pImage = mImgInd != -1 ? pImgMgr->GetLockerResource(mImgInd) : nullptr;

    if(pImage) {
        mTileWidth = pImage->getImageWidth() / mTilesCountWidth;
        mTileHeight = pImage->getImageHeight() / mTilesCountHeight;

        maTileIndexes.resize(mTilesCountWidth * mTilesCountHeight);

        for(size_t i = 0; i < maTileIndexes.size(); i++) maTileIndexes[i] = 0;
    }
    else maTileIndexes.clear();

    switch (swapmode) {
    case KeypointLayerInteractSwap::KeepNotDeformed:
        if (pImage == nullptr) break;
        mRect.elem_to = pImage->getRectView();
        break;
    case KeypointLayerInteractSwap::SetDefault:
        mRect.set_default();
        break;
    case KeypointLayerInteractSwap::NewTransform:
        if (!keypoint->swap_rect_ptr) {
            mRect.elem_to = *keypoint->swap_rect_ptr;
            mRect.reset_tracker();
        } else
            mRect.set_default();
        break;
    case KeypointLayerInteractSwap::KeepInAspect:
        break;
    }
}

inline void film::LayerTileset::pushTSetResSetter(KeypointLayerInteractTilesetResize* keypoint) {
    if(pImage) {
        mTileWidth = pImage->getImageWidth() / keypoint->new_tile_count_width;
        mTileHeight = pImage->getImageHeight() / keypoint->new_tile_count_height;

        maTileIndexes.resize(keypoint->new_tile_count_width * keypoint->new_tile_count_height);

        for(size_t i = 0; i < maTileIndexes.size(); i++) maTileIndexes[i] = 0;
    }
    mTilesCountWidth = keypoint->new_tile_count_width;
    mTilesCountHeight = keypoint->new_tile_count_height;
}

bool film::LayerTileset::onPushSetter(KeypointLayer* keypoint) {
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosSetter<KeypointLayerInteractPos>(keypoint);                            break;
    case KeypointLayer::InteractRectPos: pushPosSetter<KeypointLayerInteractRectPos>(keypoint);                        break;
    case KeypointLayer::InteractAlpha:
    {
        const auto kp_alpha = dynamic_cast<KeypointLayerInteractAlpha*>(keypoint);
        mColorAlpha.shift_elem();
        mColorAlpha.ease_tracker.reset();
        mColorAlpha.elem_to.a = kp_alpha->alpha;
    }   break;
	case KeypointLayer::InteractColor:
    {
        const auto kp_color = dynamic_cast<KeypointLayerInteractColor*>(keypoint);
        mColorAlpha.shift_elem();
        mColorAlpha.ease_tracker.reset();
        mColorAlpha.elem_to = kp_color->color;
    }   break;
    case KeypointLayer::InteractDefaultPos:     mRect.set_default(); break;
    case KeypointLayer::InteractSwap:
        pushImgIndSetter(dynamic_cast<KeypointLayerInteractSwap*>(keypoint));
        break;
    case KeypointLayer::InteractTilesetSwap:
    {
        const auto kp_tswap = dynamic_cast<KeypointLayerInteractTilesetSwap*>(keypoint);
        maTileIndexes[kp_tswap->tileset_y * mTilesCountWidth + kp_tswap->tileset_x] = kp_tswap->swap;
    }   break;
    case KeypointLayer::InteractTilesetResize: pushTSetResSetter(dynamic_cast<KeypointLayerInteractTilesetResize*>(keypoint)); break;
    case KeypointLayer::InteractScaleMode: pImage->setScaleMode(dynamic_cast<KeypointLayerInteractScaleMode*>(keypoint)->scale); break;
    case KeypointLayer::InteractDefault:
        mRect.set_default();
        mColorAlpha.set_default();
        break;
    default:
        return true;
    }

    return false;
}

bool film::LayerTileset::onPushTracker(const LockerIndex ease_indx) {
    auto& tracker = maEases[ease_indx];
    const auto keypoint = tracker.keypoint;
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosTracker<KeypointLayerInteractPos>(ease_indx);                      break;
    case KeypointLayer::InteractRectPos: pushPosTracker<KeypointLayerInteractRectPos>(ease_indx);                  break;
    case KeypointLayer::InteractAlpha:
        pushTransitTracker(tracker, mColorAlpha);
        mColorAlpha.elem_to.a = dynamic_cast<KeypointLayerInteractAlpha*>(keypoint)->alpha;
        break;
	case KeypointLayer::InteractColor:
		pushTransitTracker(tracker, mColorAlpha);
        mColorAlpha.elem_to = dynamic_cast<KeypointLayerInteractColor*>(keypoint)->color;
        break;
    default:
        return true;
    }

    return false;
}

