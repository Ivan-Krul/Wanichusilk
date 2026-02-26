#include "FilmLayerGroup.h"

bool film::LayerGroup::join(PolyPointerList<LayerBase>::Iterator it) {
    mLockerLayers.push_back(it);
    return false;
}

bool film::LayerGroup::interact(LockerIndex group_nr, KeypointLayer* keypoint) {
    if (!keypoint->has_ease()) return mLockerLayers[group_nr]->pushSetter(keypoint);
    return mLockerLayers[group_nr]->pushTracker(dynamic_cast<KeypointLayerEase*>(keypoint));
}

bool film::LayerGroup::interactAll(KeypointLayer* keypoint) {
    bool res = true; // if every layer goes wrong, then everything is wrong
    for (auto layer : mLockerLayers) {
        if (!keypoint->has_ease()) res &= layer->pushSetter(keypoint);
        else res &= layer->pushTracker(dynamic_cast<KeypointLayerEase*>(keypoint));
    }
    return res;
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

