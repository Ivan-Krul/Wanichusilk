#include "FilmLayerGroup.h"

bool film::LayerGroup::join(PolyPointerList<LayerBase>::Iterator it) {
    mLockerLayers.push_back(it);
    return false;
}

bool film::LayerGroup::interact(KeypointLayerGroupInteract* keypoint) {
    if (!keypoint->has_ease()) return mLockerLayers[keypoint->group_nr]->pushSetter(keypoint);
    return mLockerLayers[keypoint->group_nr]->pushTracker(dynamic_cast<KeypointLayerEase*>(keypoint));
}

bool film::LayerGroup::interactAll(KeypointLayerGroupSharedInteract* keypoint) {
    for (auto layer : mLockerLayers) {
        if (!keypoint->has_ease()) layer->pushSetter(keypoint);
        layer->pushTracker(dynamic_cast<KeypointLayerEase*>(keypoint));
    }
    return false;
}

bool film::LayerGroup::detach(PolyPointerList<LayerBase>::Iterator it) {
    auto iter = mLockerLayers.begin();
    while (iter != mLockerLayers.end()) {
        if (*iter == it) {
            mLockerLayers.erase(iter);
            return false;
        }
    }
    return true;
}

