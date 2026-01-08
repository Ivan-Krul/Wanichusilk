#include "FilmLayerAnimation.h"

film::LayerAnimation::LayerAnimation(Clock* clock, AnimationManager* animmgr, AnimationIndex animind) {
    pAnimMgr = animmgr;
    setClock(clock);

    mAnimInd.elem_to = animind;
    pAnimation = animind != -1 ? pAnimMgr->GetLockerResource(animind) : nullptr;
    mRect.elem_to = pAnimation ? pAnimation->getRectRes() : SDL_FRect{ 0.f };
    mAlpha.elem_to = 255;
    mTimeMult.elem_to = 1.f;

    mRect.ease_tracker.setClock(clock);
    mAlpha.ease_tracker.setClock(clock);
    mAnimInd.ease_tracker.setClock(clock);
    mTimeMult.ease_tracker.setClock(clock);

    mRect.reset_tracker();
    mAnimInd.reset_tracker();
}

void film::LayerAnimation::update() {
    if (maEases.isEmpty()) return;
    mRect.ease_tracker.update();
    mAlpha.ease_tracker.update();
    mAnimInd.ease_tracker.update();
    mTimeMult.ease_tracker.update();

    auto it = maEases.begin();
    while (it != maEases.end()) {
        if (it->ease->isEnded()) {
            if (it->ease == &(mAnimInd.ease_tracker)) // handle the swap logic
                finalizeSwap(it);
            it = maEases.popFromLocker(it);
        }
        else it++;
    }
}

void film::LayerAnimation::render() const {
    SDL_FRect rect = { 0.f };
    uint8_t alpha = 0;
    float time_mult = 0.f;

    const auto res_rect = computeRectRender(mRect, &rect);

    if (mAlpha.is_progress())
        alpha = lerp(mAlpha.ease_tracker, mAlpha.elem_from, mAlpha.elem_to);
    else alpha = mAlpha.elem_to;
    if (mTimeMult.is_progress())
        time_mult = lerp(mTimeMult.ease_tracker, mTimeMult.elem_from, mTimeMult.elem_to);
    else time_mult = mTimeMult.elem_to;

    if (mAnimInd.is_progress())
        renderSwap(res_rect, alpha, time_mult);
    else if (areAllTransitParamDefault())
        pAnimation->render();
    else
        pAnimation->renderRaw(res_rect, alpha, time_mult);
}

void film::LayerAnimation::clear() {
    maEases.clear();
    mRect.clear();
    mAlpha.clear();
    mAnimInd.clear();
    mTimeMult.clear();

    pAnimation = nullptr;
}

inline TimerStep film::LayerAnimation::getLongestWaiting() const noexcept {
    TimerStep longest = clockfunc::max(mAlpha.ease_tracker.getLimiter(), mRect.ease_tracker.getLimiter());
    longest = clockfunc::max(mAnimInd.ease_tracker.getLimiter(), longest);
    longest = clockfunc::max(mTimeMult.ease_tracker.getLimiter(), longest);
    return longest;
}

void film::LayerAnimation::pushAnimIndSetter(KeypointLayer* keypoint) {
    const auto kp = dynamic_cast<KeypointLayerInteractSwap*>(keypoint);
    KeypointLayerSwap::SwapMode swapmode = kp->swap;

    mRect.shift_elem();

    mAnimInd.reset_tracker();
    mAnimInd.shift_elem();
    mAnimInd.elem_to = kp->indx;
    pAnimation = mAnimInd.elem_to != -1 ? pAnimMgr->GetLockerResource(mAnimInd.elem_to) : nullptr;

    switch (swapmode) {
    case KeypointLayerSwap::KeepNotDeformed:
        if (pAnimation == nullptr) break;
        mRect.elem_to = pAnimation->getRectRes();
        break;
    case KeypointLayerSwap::SetDefault:
        mRect.set_default();
        break;
    case KeypointLayerSwap::NewTransform:
        if (!kp->swap_rect_ptr) {
            mRect.elem_to = *kp->swap_rect_ptr;
            mRect.reset_tracker();
        }
        else mRect.set_default();
        break;
    }
}


bool film::LayerAnimation::onPushSetter(KeypointLayer* keypoint) {
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosSetter<KeypointLayerInteractPos>(keypoint);     break;
    case KeypointLayer::InteractRectPos: pushPosSetter<KeypointLayerInteractRectPos>(keypoint); break;
    case KeypointLayer::InteractAlpha:
        mAlpha.shift_elem();
        mAlpha.ease_tracker.reset();
        mAlpha.elem_to = dynamic_cast<KeypointLayerInteractAlpha*>(keypoint)->alpha;
        break;
    case KeypointLayer::InteractDefaultPos:     mRect.set_default(); break;
    case KeypointLayer::InteractSwap: pushAnimIndSetter(keypoint);   break;
    case KeypointLayer::InteractAnimationStart: if (pAnimation->isFreezed()) pAnimation->setFreeze(false); break;
    case KeypointLayer::InteractAnimationStop:  pAnimation->setFreeze(true);    break;
    case KeypointLayer::InteractAnimationLoop:   pAnimation->setLooping(true);  break;
    case KeypointLayer::InteractAnimationUnloop: pAnimation->setLooping(false); break;
    case KeypointLayer::InteractAnimationSpeed:
        mTimeMult.shift_elem();
        mTimeMult.ease_tracker.reset();
        mTimeMult.elem_to = dynamic_cast<KeypointLayerInteractAnimationSpeed*>(keypoint)->speed;
        break;
    case KeypointLayer::InteractDefault:
        mRect.set_default();
        mAlpha.set_default();
        mAnimInd.set_default();
        mTimeMult.set_default();
        break;
    default:
        return true;
    }

    return false;
}

inline void film::LayerAnimation::renderSwap(const SDL_FRect* res_rect, uint8_t max_alpha, float time_mult) const {
    const float progress = mAnimInd.ease_tracker;
    if (mAnimInd.elem_from != -1) pAnimMgr->GetLockerResource(mAnimInd.elem_from)->renderRaw(res_rect, max_alpha * (1.f - progress), time_mult);
    if (mAnimInd.elem_to == -1) return;

    const auto tracked_kp = dynamic_cast<KeypointLayerInteractTransparentSwap*>(maEases.at(mAnimInd.unused_padding).keypoint);

    switch (tracked_kp->swap) {
    default: _FALLTHROUGH;
    case KeypointLayerSwap::KeepInAspect:
        pAnimation->renderRaw(res_rect, max_alpha * progress, time_mult);
        break;
    case KeypointLayerSwap::KeepNotDeformed:
        pAnimation->render();
        break;
    case KeypointLayerSwap::SetDefault:
        pAnimation->renderRaw(nullptr, max_alpha * progress, time_mult);
        break;
    case KeypointLayerSwap::NewTransform:
        pAnimation->renderRaw(tracked_kp->swap_rect_ptr.get(), max_alpha * progress, time_mult);
        break;
    }
}

inline void film::LayerAnimation::finalizeSwap(LockerSimple<LayerBase::Tracker>::Iterator iter) {
    const auto keypoint = dynamic_cast<KeypointLayerInteractTransparentSwap*>(iter->keypoint);
    const auto swapmode = keypoint->swap;

    mRect.shift_elem();

    switch (swapmode) {
    case KeypointLayerSwap::KeepNotDeformed:
        if (pAnimation == nullptr) break;
        mRect.elem_to = pAnimation->getRectRes();
        break;
    case KeypointLayerSwap::SetDefault:
        mRect.set_default();
        break;
    case KeypointLayerSwap::NewTransform:
        if (keypoint->swap_rect_ptr)
            mRect.set_default();
        else mRect.elem_to = *keypoint->swap_rect_ptr;
        break;
    }
}

bool film::LayerAnimation::onPushTracker(const LockerIndex ease_indx) {
    auto& tracker = maEases[ease_indx];
    const auto keypoint = tracker.keypoint;
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosTracker<KeypointLayerInteractPos>(ease_indx);     break;
    case KeypointLayer::InteractRectPos: pushPosTracker<KeypointLayerInteractRectPos>(ease_indx); break;
    case KeypointLayer::InteractAlpha:
        pushTransitTracker(tracker, mAlpha);
        mAlpha.elem_to = dynamic_cast<KeypointLayerInteractAlpha*>(keypoint)->alpha;
        break;
    case KeypointLayer::InteractAnimationSpeed:
        pushTransitTracker(tracker, mTimeMult);
        mTimeMult.elem_to = dynamic_cast<KeypointLayerInteractAnimationSpeed*>(keypoint)->speed;
        break;
    case KeypointLayer::InteractTransparentSwap:
    {
        pushTransitTracker(tracker, mAnimInd);
        mAnimInd.elem_to = dynamic_cast<KeypointLayerInteractTransparentSwap*>(keypoint)->indx;
        mAnimInd.unused_padding = ease_indx;
        pAnimation = mAnimInd.elem_to != -1 ? pAnimMgr->GetLockerResource(mAnimInd.elem_to) : nullptr;
    }   break;
    default:
        return true;
    }

    return false;
}
