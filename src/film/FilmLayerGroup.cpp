#include "FilmLayerGroup.h"

film::LayerGroup::LayerGroup(Clock* clock, Layerist* layerist) {
    setClock(clock);
}

bool film::LayerGroup::join(LockerIndex layerindex) {
    mLockerLayers.push_back(layerindex);
    return false;
}

bool film::LayerGroup::onPushSetter(KeypointLayer* keypoint) {
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
