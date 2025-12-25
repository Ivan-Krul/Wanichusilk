#include "FilmLayerAnimation.h"

film::LayerAnimation::LayerAnimation(Clock* clock, AnimationManager* animmgr, AnimationIndex animind) {
    pAnimMgr = animmgr;
    setClock(clock);

    mAnimInd.elem_to = animind;
    pAnimation = animind != -1 ? pAnimMgr->GetLockerAnimation(animind) : nullptr;
    mRect.elem_to = pAnimation ? pAnimation->getRectRes() : SDL_FRect{ 0.f };;

    mPart.ease_tracker.setClock(clock);
    mRect.ease_tracker.setClock(clock);
    mAlpha.ease_tracker.setClock(clock);
    mAnimInd.ease_tracker.setClock(clock);
    mTimeMult.ease_tracker.setClock(clock);

    mRect.reset_tracker();
    mAnimInd.reset_tracker();
}

inline void film::LayerAnimation::clear() {
    maEases.clear();
    mPart.clear();
    mRect.clear();
    mAlpha.clear();
    mAnimInd.clear();
    mTimeMult.clear();

    pAnimation = nullptr;
}

inline TimerStep film::LayerAnimation::getLongestWaiting() const noexcept {
    TimerStep longest = clockfunc::max(mPart.ease_tracker.getLimiter(), mRect.ease_tracker.getLimiter());
    longest = clockfunc::max(mAlpha.ease_tracker.getLimiter(), longest);
    longest = clockfunc::max(mAnimInd.ease_tracker.getLimiter(), longest);
    longest = clockfunc::max(mTimeMult.ease_tracker.getLimiter(), longest);
    return longest;
}

bool film::LayerAnimation::onPushSetter(KeypointLayer* keypoint) {
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosSetter<KeypointLayerInteractPos>(keypoint); break;
    case KeypointLayer::InteractRectPos: pushPosSetter<KeypointLayerInteractRectPos>(keypoint); break;
    case KeypointLayer::InteractPartPos: pushOtherPosSetter<KeypointLayerInteractPartitionPos>(keypoint, mPart); break;
    case KeypointLayer::InteractAlpha:
        mAlpha.shift_elem();
        mAlpha.ease_tracker.reset();
        mAlpha.elem_to = dynamic_cast<KeypointLayerInteractAlpha*>(keypoint)->alpha;
        break;
    case KeypointLayer::InteractDefaultPos:     mRect.set_default(); break;
    case KeypointLayer::InteractDefaultPartPos: mPart.set_default(); break;
    case KeypointLayer::InteractSwap: assert(false); break;
    case KeypointLayer::InteractAnimationStart: if (pAnimation) { if (pAnimation->isFreezed()) pAnimation->setFreeze(false); else pAnimation->start(mTimeMult.elem_to); } break;
    case KeypointLayer::InteractAnimationStop:  pAnimation->setFreeze(true);  break;
    case KeypointLayer::InteractAnimationLoop:   pAnimation->setLooping(true);  break;
    case KeypointLayer::InteractAnimationUnloop: pAnimation->setLooping(false); break;
    case KeypointLayer::InteractAnimationSpeed:
        mTimeMult.shift_elem();
        mTimeMult.ease_tracker.reset();
        mTimeMult.elem_to = dynamic_cast<KeypointLayerInteractAnimationSpeed*>(keypoint)->speed;
        break;
    case KeypointLayer::InteractDefault:
        mRect.set_default();
        mPart.set_default();
        mAlpha.set_default();
        mAnimInd.set_default();
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
    case KeypointLayer::InteractPos:     pushPosTracker<KeypointLayerInteractPos>(ease_indx);                      break;
    case KeypointLayer::InteractRectPos: pushPosTracker<KeypointLayerInteractRectPos>(ease_indx);                  break;
    case KeypointLayer::InteractPartPos: pushOtherPosTracker<KeypointLayerInteractPartitionPos>(ease_indx, mPart); break;
    case KeypointLayer::InteractAlpha:
        pushTransitTracker(tracker, mAlpha);
        mAlpha.elem_to = dynamic_cast<KeypointLayerInteractAlpha*>(keypoint)->alpha;
        break;
    case KeypointLayer::InteractTransparentSwap:
    {
        pushTransitTracker(tracker, mAnimInd);
        mAnimInd.elem_to = dynamic_cast<KeypointLayerInteractTransparentSwap*>(keypoint)->texindx;
        mAnimInd.unused_padding = ease_indx;
        pAnimation = mAnimInd.elem_to != -1 ? pAnimMgr->GetLockerAnimation(mAnimInd.elem_to) : nullptr;
    }   break;
    default:
        return true;
    }

    return false;
}
