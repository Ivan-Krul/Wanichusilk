#include "FilmLayerist.h"

void FilmLayerist::registerLayerKeypoint(FilmKeypointLayer* keypoint) {
    assert(keypoint->type().specific_type >= FilmKeypointLayer::Add && keypoint->type().specific_type <= FilmKeypointLayer::Remove);
    assert(keypoint->layerindx < maLayers.size() || keypoint->type().specific_type == FilmKeypointLayer::Add);

    LayerIndex li = (keypoint->type().specific_type != FilmKeypointLayer::Add) ? keypoint->layerindx : -1;

    if (keypoint->type().specific_type >= FilmKeypointLayer::InteractPos && keypoint->type().specific_type <= FilmKeypointLayer::InteractDefault) {
        registerKeypointInteraction(li, keypoint);
        return;
    }

    switch (keypoint->type().specific_type) { // it's mess now, but I must go further
    case FilmKeypointLayer::Add:             registerLayerKeypointAdd(dynamic_cast<FilmKeypointLayerAdd*>(keypoint)); break;
    case FilmKeypointLayer::Enable: maActiveLayerIndexes.push_back(maLayers.begin() + li); break;
    case FilmKeypointLayer::Disable:
    {
        auto iter = maActiveLayerIndexes.begin() + li;
        if(iter != maActiveLayerIndexes.end()) maActiveLayerIndexes.erase(iter);
    }   break;
    case FilmKeypointLayer::Remove:
    {
        auto iter = maActiveLayerIndexes.begin() + li;
        if (iter != maActiveLayerIndexes.end()) maActiveLayerIndexes.erase(iter);
        maLayers.erase(maLayers.begin() + li);
    }   break;
    case FilmKeypointLayer::Await: assert(false); break; // useless?
    default: assert(false); break;
    }

}

TimerStep FilmLayerist::getLongestWaiting() const {
    TimerStep longest;
    auto iter = maLayers.cbegin();
    for (iter; iter != maLayers.cend(); iter++) {
        longest = ClockFunc::max(longest, iter->getLongestWaiting());
    }
    
    return longest;
}

void FilmLayerist::update() {
    auto it = maLayers.begin();
    while (it != maLayers.end()) {
        it->update();
        it++;
    }
}

void FilmLayerist::render() {
    for (auto& active : maActiveLayerIndexes) {
        active->render();
    }
}

bool FilmLayerist::isWaiting() const {
    for (auto it = maLayers.cbegin(); it != maLayers.cend(); it++) {
        if (!it->isWaiting()) return false;
    }
    return true;
}

void FilmLayerist::registerLayerKeypointAdd(FilmKeypointLayerAdd* keypoint) {
    switch (keypoint->layertype()) {
    case FilmKeypointLayerAdd::Texture:
    {
        const auto conv_kp = dynamic_cast<FilmKeypointLayerAddTexture*>(keypoint);
        maLayers.emplace_back<FilmLayerTexture>(pClock, pTexMgr, conv_kp->texind);
    }   break;
    default:
        break;
    }
}

void FilmLayerist::registerKeypointInteraction(LayerIndex li, FilmKeypointLayer* keypoint) {
    auto iter = maLayers.begin() + li;

    if (!keypoint->has_ease()) {
        iter->pushSetter(keypoint);
        return;
    }

    switch (keypoint->type().specific_type) {
    case FilmKeypointLayer::InteractPos:             iter->pushTracker(dynamic_cast<FilmKeypointLayerInteractPos*>(keypoint)); break;
    case FilmKeypointLayer::InteractRectPos:         iter->pushTracker(dynamic_cast<FilmKeypointLayerInteractRectPos*>(keypoint)); break;
    case FilmKeypointLayer::InteractPartPos:         iter->pushTracker(dynamic_cast<FilmKeypointLayerInteractPartitionPos*>(keypoint)); break;
    case FilmKeypointLayer::InteractDefaultPos:      iter->pushTracker(dynamic_cast<FilmKeypointLayerInteractDefaultPos*>(keypoint)); break;
    case FilmKeypointLayer::InteractDefaultPartPos:  iter->pushTracker(dynamic_cast<FilmKeypointLayerInteractDefaultPartitionPos*>(keypoint)); break;
    case FilmKeypointLayer::InteractAlpha:           iter->pushTracker(dynamic_cast<FilmKeypointLayerInteractAlpha*>(keypoint)); break;
    case FilmKeypointLayer::InteractTransparentSwap: iter->pushTracker(dynamic_cast<FilmKeypointLayerInteractTransparentSwap*>(keypoint)); break;
    }
}
