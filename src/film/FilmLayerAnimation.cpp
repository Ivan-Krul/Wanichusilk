#include "FilmLayerAnimation.h"

film::LayerAnimation::LayerAnimation(Clock* clock, AnimationManager* animmgr, AnimationIndex animind) {
    pAnimMgr = animmgr;
    setClock(clock);

    mAnimInd = animind;
    pAnimation = animind != -1 ? pAnimMgr->GetLockerResource(animind) : nullptr;
    if (pAnimation) {
        pAnimation->lockChange();
        pAnimation->setFreeze(true);
        pAnimation->start(1.f);
    }
    mRect.elem_to = pAnimation ? pAnimation->getRectRes() : SDL_FRect{ 0.f };
    mAlpha.elem_to = 255;
    mTimeMult.elem_to = 1.f;

    mRect.ease_tracker.setClock(clock);
    mAlpha.ease_tracker.setClock(clock);
    mTimeMult.ease_tracker.setClock(clock);

    mRect.reset_tracker();
}

void film::LayerAnimation::update() {
    if (maEases.isEmpty()) return;
    mRect.ease_tracker.update();
    mAlpha.ease_tracker.update();
    mTimeMult.ease_tracker.update();

    auto it = maEases.begin();
    while (it != maEases.end()) {
        if (it->ease->isEnded())
            it = maEases.popFromLocker(it);
        else it++;
    }
}

void film::LayerAnimation::render() const {
    assert(pAnimation);
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

    if (areAllTransitParamDefault())
        pAnimation->render();
    else
        pAnimation->renderRaw(res_rect, alpha, time_mult);
}

void film::LayerAnimation::clear() {
    maEases.clear();
    mRect.clear();
    mAlpha.clear();
    mTimeMult.clear();

    mAnimInd = -1;
    pAnimation = nullptr;
}

inline TimerStep film::LayerAnimation::getLongestWaiting() const noexcept {
    TimerStep longest = clockfunc::max(mAlpha.ease_tracker.getLimiter(), mRect.ease_tracker.getLimiter());
    longest = clockfunc::max(mTimeMult.ease_tracker.getLimiter(), longest);
    return longest;
}

void film::LayerAnimation::pushAnimIndSetter(KeypointLayer* keypoint) {
    const auto kp = dynamic_cast<KeypointLayerInteractSwap*>(keypoint);
    KeypointLayerSwap::SwapMode swapmode = kp->swap;

    mRect.shift_elem();

    mAnimInd = kp->indx;
    pAnimation = mAnimInd != -1 ? pAnimMgr->GetLockerResource(mAnimInd) : nullptr;
    if (pAnimation) {
        pAnimation->lockChange();
        pAnimation->setFreeze(true);
        pAnimation->start(1.f);
    }

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
        mTimeMult.set_default();
        break;
    default:
        return true;
    }

    return false;
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
    default:
        return true;
    }

    return false;
}
