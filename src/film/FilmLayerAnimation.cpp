#include "FilmLayerAnimation.h"

film::LayerAnimation::LayerAnimation(Clock* clock, AnimationManager* animmgr, AnimationIndex animind) {
    pAnimMgr = animmgr;
    setClock(clock);

    mAnimInd.elem_to = animind;
    mRect.elem_to = pAnimMgr->GetLockerTexture(animind).getRectRes();

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
}

bool film::LayerAnimation::onPushSetter(KeypointLayer* keypoint) {
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     break;//pushPosSetter(dynamic_cast<KeypointLayerInteractRect*>(keypoint), Pos);     break;
    case KeypointLayer::InteractRectPos: break;//pushPosSetter(dynamic_cast<KeypointLayerInteractRect*>(keypoint), RectPos); break;
    case KeypointLayer::InteractPartPos: break;//pushPosSetter(dynamic_cast<KeypointLayerInteractRect*>(keypoint), PartPos); break;
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
        //pushTexIndSetter(dynamic_cast<KeypointLayerInteractSwap*>(keypoint));
        break;
    case KeypointLayer::InteractAnimationStart: if (pAnimation) { if (pAnimation->isFreezed()) pAnimation->setFreeze(false); else pAnimation->start(mTimeMult.elem_to); } break;
    case KeypointLayer::InteractAnimationStop:  pAnimation->setFreeze(true);  break;
    case KeypointLayer::InteractAnimationLoop:   pAnimation->setLooping(true);  break;
    case KeypointLayer::InteractAnimationUnloop: pAnimation->setLooping(false); break;
    case KeypointLayer::InteractAnimationSpeed:
    {
        const auto kp_speed = dynamic_cast<KeypointLayerInteractAnimationSpeed*>(keypoint);
        mTimeMult.shift_elem();
        mTimeMult.ease_tracker.reset();
        mTimeMult.elem_to = kp_speed->speed;
    }   break;

    default:
        return true;
    }

    return false;
}
