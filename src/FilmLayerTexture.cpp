#include "FilmLayerTexture.h"

void FilmLayerTexture::update() {

}

void FilmLayerTexture::render() const {
}

inline void FilmLayerTexture::clear() {
}

void FilmLayerTexture::pushPosSetter(const FilmKeypointLayerInteractRect* keypoint, PosChangeEnum change) {
    TransitParam<SDL_FRect>* pos_ptr;
    pos_ptr = change == PartPos ? &mPart : &mRect;

    pos_ptr->shift_elem();
    if (change == Pos) {
        pos_ptr->elem_to.x = keypoint->rect.x;
        pos_ptr->elem_to.y = keypoint->rect.y;
    } else pos_ptr->elem_to = keypoint->rect;
}

void FilmLayerTexture::pushTexIndSetter(FilmKeypointLayerInteractSwap* keypoint) {
    FilmKeypointLayerSwap::SwapMode swapmode = keypoint->swap;

    switch (swapmode) {
    case FilmKeypointLayerSwap::KeepNotDeformed:
        const auto tex = pTexMgr->GetLockerTexture(keypoint->texindx);
        mRect.elem_to = tex.getRectRes();
        mPart.elem_to = tex.getRectPart();
        break;
    case FilmKeypointLayerSwap::SetDefault:
        mRect.ease_tracker.setDefault();
        mPart.ease_tracker.setDefault();
        break;
    case FilmKeypointLayerSwap::NewTransform:
        if (keypoint->swap_rect_ptr)
            mRect.ease_tracker.setDefault();
        else mRect.elem_to = *keypoint->swap_rect_ptr;

        if (keypoint->swap_part_ptr)
            mPart.ease_tracker.setDefault();
        else mPart.elem_to = *keypoint->swap_part_ptr;
        break;
    }
}

bool FilmLayerTexture::onPushSetter(const FilmKeypointLayer* keypoint) {
    switch (keypoint->type().specific_type) {
    case FilmKeypointLayer::InteractPos:     pushPosSetter(dynamic_cast<FilmKeypointLayerInteractRect*>(keypoint), Pos);     break;
    case FilmKeypointLayer::InteractRectPos: pushPosSetter(dynamic_cast<FilmKeypointLayerInteractRect*>(keypoint), RectPos); break;
    case FilmKeypointLayer::InteractPartPos: pushPosSetter(dynamic_cast<FilmKeypointLayerInteractRect*>(keypoint), PartPos); break;
    case FilmKeypointLayer::InteractAlpha:
        const auto kp_alpha = dynamic_cast<FilmKeypointLayerInteractAlpha*>(keypoint);
        mAlpha.shift_elem();
        mAlpha.ease_tracker.reset();
        mAlpha.elem_to = kp_alpha->alpha;
        break;
    case FilmKeypointLayer::InteractDefaultPos:     mRect.ease_tracker.setDefault(); break;
    case FilmKeypointLayer::InteractDefaultPartPos: mPart.ease_tracker.setDefault(); break;
    case FilmKeypointLayer::InteractSwap:
        pushTexIndSetter(keypoint);
        break;
    default:
        return true;
    }

    return false;
}

bool FilmLayerTexture::onPushTracker(LockerIndex ease_indx) {
    return false;
}
