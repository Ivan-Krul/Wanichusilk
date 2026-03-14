#include "FilmLayerSprite.h"
#include "../Logger.h"

#include <algorithm>

bool film::LayerSprite::registerKeypoint(PolyPointerList<LayerBase>& layers, std::vector<PolyPointerList<LayerBase>::Iterator>& ali, LayerIndex li, KeypointLayer* keypoint) {
    switch (keypoint->type().specific_type) {
    case KeypointLayer::SpriteJoin: {
        const auto kp = dynamic_cast<KeypointLayerSpriteJoin*>(keypoint);
        if (kp->joining_layerindx == li) {
            Logger log(DEFAULT_LOG_PATH);
            log.logWarningIn(__FUNCTION__, "index for joining sprite and layer index are same");
            return true;
        }
        const auto it = layers.begin() + kp->joining_layerindx;
        return join(it);
    }
    case KeypointLayer::SpriteInteract: {
        const auto kp = dynamic_cast<KeypointLayerSpriteInteract*>(keypoint);
        return interact(kp->sprite_nr, kp->keypoint.get());
    }
    case KeypointLayer::SpriteSwap:
        if (mSwapIndex != -1 && mSwapIndex < mLockerLayers.size()) {
            auto iter = std::find(ali.begin(), ali.end(), mLockerLayers[mSwapIndex]);
            if (iter == ali.end()) return true;
            mSwapIndex = dynamic_cast<KeypointLayerSpriteSwap*>(keypoint)->sprite_nr;
            *iter = mLockerLayers[mSwapIndex];
        }
        else {
            mSwapIndex = dynamic_cast<KeypointLayerSpriteSwap*>(keypoint)->sprite_nr;
            if (mSwapIndex != -1 && mSwapIndex < mLockerLayers.size()) {
                auto iter = std::find(ali.begin(), ali.end(), mLockerLayers[mSwapIndex]);
                if (iter == ali.end()) ali.push_back(mLockerLayers[mSwapIndex]);
                else *iter = mLockerLayers[mSwapIndex];
            }
        }
        return false;
    case KeypointLayer::SpriteSharedInteract:
        return interactAll(dynamic_cast<KeypointLayerSpriteSharedInteract*>(keypoint)->keypoint.get());
    case KeypointLayer::SpriteDetach: {
        const auto kp = dynamic_cast<KeypointLayerSpriteDetach*>(keypoint);
        if (kp->detaching_layerindx == li) {
            Logger log(DEFAULT_LOG_PATH);
            log.logWarningIn(__FUNCTION__, "index for detaching sprite and layer index are same");
            return true;
        }
        if (kp->detaching_layerindx == mSwapIndex) mSwapIndex = -1;
        if (kp->detaching_layerindx < mSwapIndex) mSwapIndex--;
        return detach(layers.begin() + kp->detaching_layerindx);
    }
    }
    return false;
}
