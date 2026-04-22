#include "FilmLayerImage.h"

film::LayerImage::LayerImage(Clock* clock, ImageManager* imgmgr, LockerIndex imgind) {
    pImgMgr = imgmgr;
    setClock(clock);

    mImgInd = imgind;
    pImage = imgind != -1 ? pImgMgr->GetLockerResource(imgind) : nullptr;
    mRect.elem_to = pImage ? pImage->getRectView() : SDL_FRect{ 0.f };
    mColorAlpha.elem_to = { 255, 255, 255, 255 };

    mSnap.ease_tracker.setClock(clock);
    mRect.ease_tracker.setClock(clock);
    mColorAlpha.ease_tracker.setClock(clock);

    mRect.reset_tracker();
}

void film::LayerImage::update() {
    if (maEases.isEmpty()) return;
    mSnap.ease_tracker.update();
    mColorAlpha.ease_tracker.update();

    removePassedEases();
}

void film::LayerImage::render() const {
	if (mImgInd == -1) return;
	
    SDL_FRect view = { 0.f };
    SDL_FRect snap = { 0.f };

    const auto res_view = computeRectRender(mRect, &view);
    const auto res_snap = computeRectRender(mSnap, &snap);
	
	if(res_view) pImage->setRectView(*res_view);

	if(res_snap) pImage->setRectSnap(*res_snap);
	else         pImage->turnOffSnap();

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

void film::LayerImage::clear() {
    maEases.clear();
    mSnap.clear();
    mRect.clear();
    mColorAlpha.clear();
    mImgInd = -1;

    pImage = nullptr;
}

inline TimerStep film::LayerImage::getLongestWaiting() const noexcept {
    TimerStep longest = clockfunc::max(mSnap.ease_tracker.getLimiter(), mRect.ease_tracker.getLimiter());
    longest = clockfunc::max(mColorAlpha.ease_tracker.getLimiter(), longest);
    return longest;
}

void film::LayerImage::pushImgIndSetter(KeypointLayerInteractSwap* keypoint) {
    KeypointLayerInteractSwap::SwapMode swapmode = keypoint->swap;

    mRect.shift_elem();
    mSnap.shift_elem();

    mImgInd = keypoint->indx;
    pImage = mImgInd != -1 ? pImgMgr->GetLockerResource(mImgInd) : nullptr;

    switch (swapmode) {
    case KeypointLayerInteractSwap::KeepNotDeformed:
        if (pImage == nullptr) break;
        mRect.elem_to = pImage->getRectView();
        mSnap.elem_to = pImage->getRectSnap();
        break;
    case KeypointLayerInteractSwap::SetDefault:
        mRect.set_default();
        mSnap.set_default();
        break;
    case KeypointLayerInteractSwap::NewTransform:
        if (!keypoint->swap_rect_ptr) {
            mRect.elem_to = *keypoint->swap_rect_ptr;
            mRect.reset_tracker();
        } else
            mRect.set_default();

        if (keypoint->swap_part_ptr) {
            mSnap.elem_to = *keypoint->swap_part_ptr;
            mSnap.reset_tracker();
        } else
            mSnap.set_default();
        break;
    case KeypointLayerInteractSwap::KeepInAspect:
        break;
    }
}

bool film::LayerImage::onPushSetter(KeypointLayer* keypoint) {
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosSetter<KeypointLayerInteractPos>(keypoint);                            break;
    case KeypointLayer::InteractRectPos: pushPosSetter<KeypointLayerInteractRectPos>(keypoint);                        break;
    case KeypointLayer::InteractSnapPos: pushOtherPosSetter<film::KeypointLayerInteractSnapPos>(keypoint, mSnap); break;
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
    case KeypointLayer::InteractDefaultSnapPos: mSnap.set_default(); break;
    case KeypointLayer::InteractSwap:
        pushImgIndSetter(dynamic_cast<KeypointLayerInteractSwap*>(keypoint));
        break;
    case KeypointLayer::InteractScaleMode: pImage->setScaleMode(dynamic_cast<KeypointLayerInteractScaleMode*>(keypoint)->scale); break;
    case KeypointLayer::InteractDefault:
        mRect.set_default();
        mSnap.set_default();
        mColorAlpha.set_default();
        break;
    default:
        return true;
    }

    return false;
}

bool film::LayerImage::onPushTracker(LockerIndex ease_indx) {
    auto& tracker = maEases[ease_indx];
    const auto keypoint = tracker.keypoint;
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosTracker<KeypointLayerInteractPos>(ease_indx);                      break;
    case KeypointLayer::InteractRectPos: pushPosTracker<KeypointLayerInteractRectPos>(ease_indx);                  break;
    case KeypointLayer::InteractSnapPos: pushOtherPosTracker<KeypointLayerInteractSnapPos>(ease_indx, mSnap); break;
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
