#include "FilmLayerist.h"

void film::Layerist::registerLayerKeypoint(KeypointLayer* keypoint) {
    assert(keypoint->type().specific_type >= KeypointLayer::Add && keypoint->type().specific_type <= KeypointLayer::Remove);
    assert(keypoint->layerindx < maLayers.size() || keypoint->type().specific_type == KeypointLayer::Add);

    LayerIndex li = (keypoint->type().specific_type != KeypointLayer::Add) ? keypoint->layerindx : -1;

    if (keypoint->type().specific_type >= KeypointLayer::InteractPos && keypoint->type().specific_type <= KeypointLayer::InteractDefault) {
        registerKeypointInteraction(li, keypoint);
        return;
    }

    switch (keypoint->type().specific_type) { // it's mess now, but I must go further
    case KeypointLayer::Add:             registerLayerKeypointAdd(dynamic_cast<KeypointLayerAdd*>(keypoint)); break;
    case KeypointLayer::Enable: maActiveLayerIndexes.push_back(maLayers.begin() + li); break;
    case KeypointLayer::Disable:
    {
        auto iter = maActiveLayerIndexes.begin() + li;
        if(iter != maActiveLayerIndexes.end()) maActiveLayerIndexes.erase(iter);
    }   break;
    case KeypointLayer::Remove:
    {
        auto iter = maActiveLayerIndexes.begin() + li;
        if (iter != maActiveLayerIndexes.end()) maActiveLayerIndexes.erase(iter);
        maLayers.erase(maLayers.begin() + li);
    }   break;
    case KeypointLayer::Await: assert(false); break; // useless?
    default: assert(false); break;
    }

}

TimerStep film::Layerist::getLongestWaiting() const {
    TimerStep longest;
    auto iter = maLayers.cbegin();
    for (iter; iter != maLayers.cend(); iter++) {
        longest = clockfunc::max(longest, iter->getLongestWaiting());
    }
    
    return longest;
}

void film::Layerist::update() {
    auto it = maLayers.begin();
    while (it != maLayers.end()) {
        it->update();
        it++;
    }
}

void film::Layerist::render() {
    for (auto& active : maActiveLayerIndexes) {
        active->render();
    }
}

bool film::Layerist::isWaiting() const {
    for (auto it = maLayers.cbegin(); it != maLayers.cend(); it++) {
        if (!it->isWaiting()) return false;
    }
    return true;
}

void film::Layerist::registerLayerKeypointAdd(KeypointLayerAdd* keypoint) {
    switch (keypoint->layertype()) {
    case KeypointLayerAdd::Texture:
    {
        const auto conv_kp = dynamic_cast<KeypointLayerAddTexture*>(keypoint);
        maLayers.emplace_back<LayerTexture>(pClock, pTexMgr, conv_kp->texind);
    }   break;
    default:
        break;
    }
}

void film::Layerist::registerKeypointInteraction(LayerIndex li, KeypointLayer* keypoint) {
    auto iter = maLayers.begin() + li;

    if (!keypoint->has_ease()) {
        iter->pushSetter(keypoint);
        return;
    }

    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:             iter->pushTracker(dynamic_cast<KeypointLayerInteractPos*>(keypoint)); break;
    case KeypointLayer::InteractRectPos:         iter->pushTracker(dynamic_cast<KeypointLayerInteractRectPos*>(keypoint)); break;
    case KeypointLayer::InteractPartPos:         iter->pushTracker(dynamic_cast<KeypointLayerInteractPartitionPos*>(keypoint)); break;
    case KeypointLayer::InteractDefaultPos:      iter->pushTracker(dynamic_cast<KeypointLayerInteractDefaultPos*>(keypoint)); break;
    case KeypointLayer::InteractDefaultPartPos:  iter->pushTracker(dynamic_cast<KeypointLayerInteractDefaultPartitionPos*>(keypoint)); break;
    case KeypointLayer::InteractAlpha:           iter->pushTracker(dynamic_cast<KeypointLayerInteractAlpha*>(keypoint)); break;
    case KeypointLayer::InteractTransparentSwap: iter->pushTracker(dynamic_cast<KeypointLayerInteractTransparentSwap*>(keypoint)); break;
    }
}
