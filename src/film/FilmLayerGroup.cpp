#include "FilmLayerGroup.h"
#include "../Logger.h"

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

bool film::LayerGroup::registerKeypoint(PolyPointerList<LayerBase>& layers, std::vector<PolyPointerList<LayerBase>::Iterator>& ali, LayerIndex li, KeypointLayer* keypoint) {
    switch (keypoint->type().specific_type) {
    case KeypointLayer::GroupJoin: {
        if (dynamic_cast<KeypointLayerGroupJoin*>(keypoint)->joining_layerindx == li) {
            Logger log(DEFAULT_LOG_PATH);
            log.logWarningIn(__FUNCTION__, "index for joining group and layer index are same");
            return true;
        }
        return join(layers.begin() + dynamic_cast<KeypointLayerGroupJoin*>(keypoint)->joining_layerindx);
    }
    case KeypointLayer::GroupInteract: {
        const auto kp = dynamic_cast<KeypointLayerGroupInteract*>(keypoint);
        return interact(kp->group_nr, kp->keypoint.get());
    }
    case KeypointLayer::GroupSharedInteract:
        return interactAll(dynamic_cast<KeypointLayerGroupSharedInteract*>(keypoint)->keypoint.get());
    case KeypointLayer::GroupDetach:
        if (dynamic_cast<KeypointLayerGroupDetach*>(keypoint)->detaching_layerindx == li) {
            Logger log(DEFAULT_LOG_PATH);
            log.logWarningIn(__FUNCTION__, "index for detaching group and layer index are same");
            return true;
        }
        return detach(layers.begin() + dynamic_cast<KeypointLayerGroupDetach*>(keypoint)->detaching_layerindx);
    }

    return true;
}


