#include "FilmLayerist.h"

bool film::Layerist::registerLayerKeypoint(KeypointLayer* keypoint) {
    assert(keypoint->type().specific_type >= KeypointLayer::Add && keypoint->type().specific_type <= KeypointLayer::Remove);
    assert(keypoint->layerindx < maLayers.size() || keypoint->type().specific_type == KeypointLayer::Add);

    LayerIndex li = (keypoint->type().specific_type != KeypointLayer::Add) ? keypoint->layerindx : -1;

    if (keypoint->type().specific_type >= KeypointLayer::InteractPos && keypoint->type().specific_type <= KeypointLayer::InteractDefault)
        return registerKeypointInteraction(li, keypoint);

    switch (keypoint->type().specific_type) { // it's mess now, but I must go further
    case KeypointLayer::Add:     return registerLayerKeypointAdd(dynamic_cast<KeypointLayerAdd*>(keypoint));
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
    default:
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "keypointer wasn't caught up with this change: %d.", keypoint->type().specific_type);
        break;// return true;
    }
    return false;
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

inline bool film::Layerist::registerLayerKeypointAdd(KeypointLayerAdd* keypoint) {
    auto loaderindx = keypoint->loaderind;
    switch (keypoint->layertype()) {
    case KeypointLayerAdd::Texture:
        maLayers.emplace_back<LayerTexture>(pClock, dynamic_cast<TextureManager*>(pLoader->GetManager(loaderindx)), pLoader->GetTranscription(loaderindx));
        break;
    case KeypointLayerAdd::Animation:
        maLayers.emplace_back<LayerAnimation>(pClock, dynamic_cast<AnimationManager*>(pLoader->GetManager(loaderindx)), pLoader->GetTranscription(loaderindx));
        break;
    case KeypointLayerAdd::Text:
        maLayers.emplace_back<LayerText>(pClock, dynamic_cast<TextManager*>(pLoader->GetManager(loaderindx)), pLoader->GetTranscription(loaderindx));
        break;
    default:
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "layer wasn't added.");
        return true;
    }
    return false;
}

inline bool film::Layerist::registerKeypointInteraction(LayerIndex li, KeypointLayer* keypoint) {
    auto iter = maLayers.begin() + li;

    if (!keypoint->has_ease()) return iter->pushSetter(keypoint);

    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:             return iter->pushTracker(dynamic_cast<KeypointLayerInteractPos*>(keypoint));
    case KeypointLayer::InteractRectPos:         return iter->pushTracker(dynamic_cast<KeypointLayerInteractRectPos*>(keypoint));
    case KeypointLayer::InteractPartPos:         return iter->pushTracker(dynamic_cast<KeypointLayerInteractPartitionPos*>(keypoint));
    case KeypointLayer::InteractDefaultPos:      return iter->pushTracker(dynamic_cast<KeypointLayerInteractDefaultPos*>(keypoint));
    case KeypointLayer::InteractDefaultPartPos:  return iter->pushTracker(dynamic_cast<KeypointLayerInteractDefaultPartitionPos*>(keypoint));
    case KeypointLayer::InteractAlpha:           return iter->pushTracker(dynamic_cast<KeypointLayerInteractAlpha*>(keypoint));
    case KeypointLayer::InteractColor:           return iter->pushTracker(dynamic_cast<KeypointLayerInteractColor*>(keypoint));
    case KeypointLayer::InteractAnimationSpeed:  return iter->pushTracker(dynamic_cast<KeypointLayerInteractAnimationSpeed*>(keypoint));
    default: 
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "keypointer wasn't caught up with this change: %d.", keypoint->type().specific_type);
        return true;
    }
}
