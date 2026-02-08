#include "FilmLayerTexture.h"
#include "FilmLayerAnimation.h"

film::LayerTexture::LayerTexture(Clock* clock, TextureManager* texmgr, LockerIndex texind) {
    pTexMgr = texmgr;
    setClock(clock);

    mTexInd = texind;
    pTexture = texind != -1 ? pTexMgr->GetLockerResource(texind) : nullptr;
    mRect.elem_to = pTexture ? pTexture->getRectRes() : SDL_FRect{ 0.f };
    mAlpha.elem_to = 255;

    mPart.ease_tracker.setClock(clock);
    mRect.ease_tracker.setClock(clock);
    mAlpha.ease_tracker.setClock(clock);

    mRect.reset_tracker();
}

void film::LayerTexture::update() {
    if (maEases.isEmpty()) return;
    mPart.ease_tracker.update();
    mRect.ease_tracker.update();
    mAlpha.ease_tracker.update();

    auto it = maEases.begin();
    while (it != maEases.end()) {
        if (it->ease->isEnded())
            it = maEases.popFromLocker(it);
        else it++;
    }
}

void film::LayerTexture::render() const {
    SDL_FRect rect = { 0.f };
    SDL_FRect part = { 0.f };
    uint8_t alpha = 0;

    const auto res_rect = computeRectRender(mRect, &rect);
    const auto res_part = computeRectRender(mPart, &part);

    if (mAlpha.is_progress())
        alpha = lerp(mAlpha.ease_tracker, mAlpha.elem_from, mAlpha.elem_to);
    else alpha = mAlpha.elem_to;

    if (mTexInd == -1) return;
    else if (areAllTransitParamDefault())
        pTexture->render();
    else
        pTexture->renderRaw(res_part, res_rect, alpha);
}

void film::LayerTexture::clear() {
    maEases.clear();
    mPart.clear();
    mRect.clear();
    mAlpha.clear();
    mTexInd = -1;

    pTexture = nullptr;
}

inline bool film::LayerTexture::isWaiting() const noexcept {
    return maEases.isEmpty();
}

inline TimerStep film::LayerTexture::getLongestWaiting() const noexcept {
    TimerStep longest = clockfunc::max(mPart.ease_tracker.getLimiter(), mRect.ease_tracker.getLimiter());
    longest = clockfunc::max(mAlpha.ease_tracker.getLimiter(), longest);
    return longest;
}

void film::LayerTexture::pushTexIndSetter(KeypointLayerInteractSwap* keypoint) {
    KeypointLayerSwap::SwapMode swapmode = keypoint->swap;

    mRect.shift_elem();
    mPart.shift_elem();

    mTexInd = keypoint->indx;
    pTexture = mTexInd != -1 ? pTexMgr->GetLockerResource(mTexInd) : nullptr;

    switch (swapmode) {
    case KeypointLayerSwap::KeepNotDeformed:
        if (pTexture == nullptr) break;
        mRect.elem_to = pTexture->getRectRes();
        mPart.elem_to = pTexture->getRectPart();
        break;
    case KeypointLayerSwap::SetDefault:
        mRect.set_default();
        mPart.set_default();
        break;
    case KeypointLayerSwap::NewTransform:
        if (!keypoint->swap_rect_ptr) {
            mRect.elem_to = *keypoint->swap_rect_ptr;
            mRect.reset_tracker();
        } else
            mRect.set_default();

        if (keypoint->swap_part_ptr) {
            mPart.elem_to = *keypoint->swap_part_ptr;
            mPart.reset_tracker();
        } else
            mPart.set_default();
        break;
    }
}

bool film::LayerTexture::onPushSetter(KeypointLayer* keypoint) {
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosSetter<KeypointLayerInteractPos>(keypoint);                            break;
    case KeypointLayer::InteractRectPos: pushPosSetter<KeypointLayerInteractRectPos>(keypoint);                        break;
    case KeypointLayer::InteractPartPos: pushOtherPosSetter<film::KeypointLayerInteractPartitionPos>(keypoint, mPart); break;
    case KeypointLayer::InteractAlpha:
    {
        const auto kp_alpha = dynamic_cast<KeypointLayerInteractAlpha*>(keypoint);
        mAlpha.shift_elem();
        mAlpha.ease_tracker.reset();
        mAlpha.elem_to = kp_alpha->alpha;
    }   break;
    case KeypointLayer::InteractDefaultPos:     mRect.set_default(); break;
    case KeypointLayer::InteractDefaultPartPos: mPart.set_default(); break;
    case KeypointLayer::InteractSwap:
        pushTexIndSetter(dynamic_cast<KeypointLayerInteractSwap*>(keypoint));
        break;
    case KeypointLayer::InteractDefault:
        mRect.set_default();
        mPart.set_default();
        mAlpha.set_default();
        break;
    default:
        return true;
    }

    return false;
}

bool film::LayerTexture::onPushTracker(LockerIndex ease_indx) {
    auto& tracker = maEases[ease_indx];
    const auto keypoint = tracker.keypoint;
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosTracker<KeypointLayerInteractPos>(ease_indx);                      break;
    case KeypointLayer::InteractRectPos: pushPosTracker<KeypointLayerInteractRectPos>(ease_indx);                  break;
    case KeypointLayer::InteractPartPos: pushOtherPosTracker<KeypointLayerInteractPartitionPos>(ease_indx, mPart); break;
    case KeypointLayer::InteractAlpha:
        pushTransitTracker(tracker, mAlpha);
        mAlpha.elem_to = dynamic_cast<KeypointLayerInteractAlpha*>(keypoint)->alpha;
        break;
    default:
        return true;
    }

    return false;
}
