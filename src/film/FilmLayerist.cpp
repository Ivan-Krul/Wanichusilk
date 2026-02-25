#include "FilmLayerist.h"

bool film::Layerist::registerLayerKeypoint(KeypointLayer* keypoint) {
    assert(keypoint->type().specific_type >= KeypointLayer::Add && keypoint->type().specific_type <= KeypointLayer::Remove);
    assert(keypoint->layerindx < maLayers.size() || keypoint->type().specific_type == KeypointLayer::Add);

    LayerIndex li = (keypoint->type().specific_type != KeypointLayer::Add) ? keypoint->layerindx : -1;

    if (keypoint->type().specific_type >= KeypointLayer::InteractPos && keypoint->type().specific_type <= KeypointLayer::InteractDefault)
        return registerKeypointInteraction(li, keypoint);

    if (keypoint->type().specific_type >= KeypointLayer::GroupJoin && keypoint->type().specific_type <= KeypointLayer::GroupDetach)
        return registerKeypointGroup(li, keypoint);

    //if (keypoint->type().specific_type >= KeypointLayer::SpriteJoin && keypoint->type().specific_type <= KeypointLayer::SpriteDetach);

    switch (keypoint->type().specific_type) { // it's mess now, but I must go further
    case KeypointLayer::Add:     return registerLayerKeypointAdd(dynamic_cast<KeypointLayerAdd*>(keypoint));
    case KeypointLayer::Enable: maActiveLayerIndexes.push_back(maLayers.begin() + li); break;
    case KeypointLayer::Disable:
    {
        auto iter = maActiveLayerIndexes.begin() + li;
        if(iter != maActiveLayerIndexes.end()) maActiveLayerIndexes.erase(iter);
    }   break;
    case KeypointLayer::Remove:
    {   // messy
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
    for (auto layers : maLayers) {
        layers->update();
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
    case KeypointLayerAdd::Group:
        maLayers.emplace_back<LayerGroup>();
    default:
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "layer wasn't added.");
        return true;
    }
    return false;
}

inline bool film::Layerist::registerKeypointInteraction(LayerIndex li, KeypointLayer* keypoint) {
    auto iter = maLayers.begin() + li;

    // if layerist knows that the class is easable, why layerbase should be bothered about it?
    if (!keypoint->has_ease()) return iter->pushSetter(keypoint);

    return iter->pushTracker(dynamic_cast<KeypointLayerEase*>(keypoint));
}

// because of advanced behaviour, layerist has to decipher all specific_type
inline bool film::Layerist::registerKeypointGroup(LayerIndex li, KeypointLayer* keypoint) {
    auto group = dynamic_cast<LayerGroup*>((maLayers.begin() + li).get());
    if (group) {
        Logger log(DEFAULT_LOG_PATH);
        log.logWarningIn(__FUNCTION__, "invalid layer or the layer is not a group");
    }

    switch (keypoint->type().specific_type) {
    case KeypointLayer::GroupJoin:           return group->join(maLayers.begin() + dynamic_cast<KeypointLayerGroupJoin*>(keypoint)->joining_layerindx);
    case KeypointLayer::GroupInteract:       return group->interact(dynamic_cast<KeypointLayerGroupInteract*>(keypoint));
    case KeypointLayer::GroupSharedInteract: return group->interactAll(dynamic_cast<KeypointLayerGroupSharedInteract*>(keypoint));
    case KeypointLayer::GroupDetach:         return group->detach(maLayers.begin() + dynamic_cast<KeypointLayerGroupDetach*>(keypoint)->detaching_layerindx);
    }

    return false;
}
