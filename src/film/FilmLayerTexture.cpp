#include "FilmLayerTexture.h"

film::LayerTexture::LayerTexture(Clock* clock, TextureManager* texmgr, LockerIndex texind) {
    pTexMgr = texmgr;
    setClock(clock);

    mTexInd.elem_to = texind;
    pTexture = texind != -1 ? &pTexMgr->GetLockerTexture(texind) : nullptr;
    mRect.elem_to = pTexture ? pTexture->getRectRes() : SDL_FRect{ 0.f };

    mPart.ease_tracker.setClock(clock);
    mRect.ease_tracker.setClock(clock);
    mAlpha.ease_tracker.setClock(clock);
    mTexInd.ease_tracker.setClock(clock);

    mRect.reset_tracker();
    mTexInd.reset_tracker();
}

void film::LayerTexture::update() {
    if (maEases.isEmpty()) return;
    mPart.ease_tracker.update();
    mRect.ease_tracker.update();
    mAlpha.ease_tracker.update();
    mTexInd.ease_tracker.update();

    auto it = maEases.begin();
    while (it != maEases.end()) {
        if (it->ease->isEnded()) {
            if (it->ease == &(mTexInd.ease_tracker)) // handle the swap logic
                finalizeSwap(it);
            it = maEases.popFromLocker(it);
        } else it++;
    }
}

void film::LayerTexture::render() const {
    SDL_FRect rect = { 0.f };
    SDL_FRect part = { 0.f };

    const SDL_FRect* res_rect = nullptr;
    const SDL_FRect* res_part = nullptr;

    uint8_t alpha = 0;

    if (mRect.is_default()) res_rect = nullptr;
    else {
        if (mRect.is_progress()) {
            rect = lerp_rect(mRect.elem_from, mRect.elem_to, mRect.ease_tracker);
            res_rect = &rect;
        } else res_rect = &(mRect.elem_to);
    }

    if (mPart.is_default()) res_part = nullptr;
    else {
        if (mPart.is_progress()) {
            part = lerp_rect(mPart.elem_from, mPart.elem_to, mPart.ease_tracker);
            res_part = &part;
        } else res_part = &(mPart.elem_to);
    }

    if (mAlpha.is_progress())
        alpha = lerp(mAlpha.ease_tracker, mAlpha.elem_from, mAlpha.elem_to);
    else alpha = mAlpha.elem_to;

    if (mTexInd.is_progress())
        renderSwap(res_rect, res_part, alpha);
    else if (mTexInd.elem_to == -1) return;
    else if (areAllTransitParamDefault())
        pTexture->render();
    else
        pTexture->renderRaw(res_part, res_rect, alpha);
}

inline void film::LayerTexture::clear() {
    maEases.clear();
    mPart.clear();
    mRect.clear();
    mAlpha.clear();
    mTexInd.clear();
}

inline bool film::LayerTexture::isWaiting() const noexcept {
    return maEases.isEmpty();
}

inline TimerStep film::LayerTexture::getLongestWaiting() const noexcept {
    TimerStep longest = clockfunc::max(mPart.ease_tracker.getLimiter(), mRect.ease_tracker.getLimiter());
    longest = clockfunc::max(mAlpha.ease_tracker.getLimiter(), longest);
    longest = clockfunc::max(mTexInd.ease_tracker.getLimiter(), longest);
    longest = clockfunc::max(mRect.ease_tracker.getLimiter(), longest);
    longest = clockfunc::max(mPart.ease_tracker.getLimiter(), longest);
    return longest;
}

void film::LayerTexture::pushPosSetter(const KeypointLayerInteractRect* keypoint, PosChangeEnum change) {
    TransitParam<SDL_FRect>* pos_ptr = change == PartPos ? &mPart : &mRect;

    pos_ptr->shift_elem();
    if (change == Pos) {
        pos_ptr->elem_to.x = keypoint->rect.x;
        pos_ptr->elem_to.y = keypoint->rect.y;
    } else pos_ptr->elem_to = keypoint->rect;
    pos_ptr->reset_tracker();
}

void film::LayerTexture::pushTexIndSetter(KeypointLayerInteractSwap* keypoint) {
    KeypointLayerSwap::SwapMode swapmode = keypoint->swap;

    mRect.shift_elem();
    mPart.shift_elem();

    mTexInd.reset_tracker();
    mTexInd.shift_elem();
    mTexInd.elem_to = keypoint->texindx;
    pTexture = mTexInd.elem_to != -1 ? &pTexMgr->GetLockerTexture(mTexInd.elem_to) : nullptr;

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
    case KeypointLayerSwap::NewTransform: _FALLTHROUGH
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
    case KeypointLayer::InteractPos:     pushPosSetter(dynamic_cast<KeypointLayerInteractRect*>(keypoint), Pos);     break;
    case KeypointLayer::InteractRectPos: pushPosSetter(dynamic_cast<KeypointLayerInteractRect*>(keypoint), RectPos); break;
    case KeypointLayer::InteractPartPos: pushPosSetter(dynamic_cast<KeypointLayerInteractRect*>(keypoint), PartPos); break;
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
    default:
        return true;
    }

    return false;
}

void film::LayerTexture::pushPosTracker(const LockerIndex ease_indx, PosChangeEnum change) {
    auto& tracker = maEases[ease_indx];
    const auto keypoint = dynamic_cast<KeypointLayerInteractRect*>(tracker.keypoint);
    TransitParam<SDL_FRect>* pos_ptr = change == PartPos ? &mPart : &mRect;
    pos_ptr->shift_elem();
    pos_ptr->ease_tracker.setEase(keypoint->ease_func);
    pos_ptr->ease_tracker.start(*dynamic_cast<TimerStep*>(keypoint));

    if (change == Pos) {
        pos_ptr->elem_to.x = keypoint->rect.x;
        pos_ptr->elem_to.y = keypoint->rect.y;
    } else pos_ptr->elem_to = keypoint->rect;

    tracker.ease = &(pos_ptr->ease_tracker);
}

bool film::LayerTexture::onPushTracker(LockerIndex ease_indx) {
    auto& tracker = maEases[ease_indx];
    const auto keypoint = tracker.keypoint;
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosTracker(ease_indx, Pos);     break;
    case KeypointLayer::InteractRectPos: pushPosTracker(ease_indx, RectPos); break;
    case KeypointLayer::InteractPartPos: pushPosTracker(ease_indx, PartPos); break;
    case KeypointLayer::InteractAlpha:
    {
        const auto kp_alpha = dynamic_cast<KeypointLayerInteractAlpha*>(keypoint);
        mAlpha.shift_elem();
        mAlpha.ease_tracker.setEase(kp_alpha->ease_func);
        mAlpha.ease_tracker.start(*dynamic_cast<TimerStep*>(keypoint));
        mAlpha.elem_to = kp_alpha->alpha;
        tracker.ease = &(mAlpha.ease_tracker);
    }   break;
    case KeypointLayer::InteractTransparentSwap:
    {
        const auto kp_swap = dynamic_cast<KeypointLayerInteractTransparentSwap*>(keypoint);
        mTexInd.shift_elem();
        mTexInd.ease_tracker.setEase(kp_swap->ease_func);
        mTexInd.ease_tracker.start(*dynamic_cast<TimerStep*>(keypoint));
        mTexInd.elem_to = kp_swap->texindx;
        mTexInd.unused_padding = ease_indx;
        pTexture = mTexInd.elem_to != -1 ? &pTexMgr->GetLockerTexture(mTexInd.elem_to) : nullptr;
        tracker.ease = &(mTexInd.ease_tracker);
    }   break;
    default:
        return true;
    }

    return false;
}

void film::LayerTexture::renderSwap(const SDL_FRect* res_rect, const SDL_FRect* res_part, uint8_t max_alpha) const {
    const float progress = mTexInd.ease_tracker;
    if (mTexInd.elem_from != -1) pTexMgr->GetLockerTexture(mTexInd.elem_from).renderRaw(res_part, res_rect, max_alpha * (1.f - progress));
    if (mTexInd.elem_to == -1) return;

    const auto tracked_kp = dynamic_cast<KeypointLayerInteractTransparentSwap*>(maEases.at(mTexInd.unused_padding).keypoint);

    switch (tracked_kp->swap) {
    default:
    case KeypointLayerSwap::KeepInAspect: _FALLTHROUGH
        pTexture->renderRaw(res_part, res_rect, max_alpha * progress);
        break;
    case KeypointLayerSwap::KeepNotDeformed:
        pTexture->render();
        break;
    case KeypointLayerSwap::SetDefault:
        pTexture->renderRaw(nullptr, nullptr, max_alpha * progress);
        break;
    case KeypointLayerSwap::NewTransform:
        pTexture->renderRaw(tracked_kp->swap_part_ptr.get(), tracked_kp->swap_rect_ptr.get(), max_alpha * progress);
        break;
    }
}

void film::LayerTexture::finalizeSwap(LockerSimple<LayerBase::Tracker>::Iterator iter) {
    const auto keypoint = dynamic_cast<KeypointLayerInteractTransparentSwap*>(iter->keypoint);
    const auto swapmode = keypoint->swap;

    mRect.shift_elem();
    mPart.shift_elem();

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
        if (keypoint->swap_rect_ptr)
            mRect.set_default();
        else mRect.elem_to = *keypoint->swap_rect_ptr;

        if (keypoint->swap_part_ptr)
             mPart.set_default();
        else mPart.elem_to = *keypoint->swap_part_ptr;
        break;
    }
}
