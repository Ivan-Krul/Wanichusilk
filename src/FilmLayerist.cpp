#include "FilmLayerist.h"

void FilmLayerist::registerLayerKeypoint(FilmKeypoint* keypoint) {
    assert(keypoint->type() >= FilmKeypointType::LayerAdd && keypoint->type() <= FilmKeypointType::LayerRemove);

    LockerIndex kpllocal = mKeypointPtrLocker.pushInLocker(KeypointTracker{ keypoint, *keypoint });

    LayerIndex li = (keypoint->type() != FilmKeypointType::LayerAdd) ? (mKeypointPtrLocker[kpllocal].layer_index = ((FilmKeypointLayer*)keypoint)->layerindx) : -1;

    switch (keypoint->type()) { // it's mess now, but I must go further
    case FilmKeypointType::LayerAdd:
        registerLayerKeypointAdd(keypoint, kpllocal); break;
    case FilmKeypointType::LayerInteractPos:
        registerLayerKeypointInteractPos(keypoint, li); break;
    case FilmKeypointType::LayerInteractSourcePos:
        maLayers[li].src = ((FilmKeypointLayerInteractSourcePos*)keypoint)->src_pos;
        maLayers[li].use_from_manager = false;
        break;
    case FilmKeypointType::LayerInteractAlpha:
        registerLayerKeypointInteractAlpha(keypoint, li); break;
    case FilmKeypointType::LayerInteractTransparentSwap:
        maLayers[li].ease_progress.swap = 0.f;
        maLayers[li].change.swap = ((maLayers[li].ease_func_swap = ((FilmKeypointLayerInteractTransparentSwap*)keypoint)->ease_func) == nullptr);
    case FilmKeypointType::LayerInteractSwap: [[fallthrough]]
        maLayers[li].texind = ((FilmKeypointLayerInteractSwap*)keypoint)->texindx;
        maLayers[li].rect = pFilmScene->getTextureManager()->GetLockerTexture(pFilmScene->getTextureIndex(maLayers[li].texind)).getRect();
        maLayers[li].use_from_manager = true;
        break;
    case FilmKeypointType::LayerEnable:
        maActiveLayerIndexes.push_back(li);
        break;
    case FilmKeypointType::LayerDisable:
        maActiveLayerIndexes.erase(std::find(maActiveLayerIndexes.begin(), maActiveLayerIndexes.end(), li));
        break;
    case FilmKeypointType::LayerInteractDefault:
        maLayers[li].use_from_manager = true;
        break;
    case FilmKeypointType::LayerAwait: break;
    case FilmKeypointType::LayerRemove:
    {
        maLayers.erase(maLayers.begin() + li);
        auto iter = std::find(maActiveLayerIndexes.begin(), maActiveLayerIndexes.end(), li);
        maActiveLayerIndexes.erase(iter);
        mKeypointPtrLocker.popFromLocker(li);
    }
    default: break;
    }

}

void FilmLayerist::update() {
    for (auto& tracker : mKeypointPtrLocker) {
        float procent = updateTimeProcenting(tracker);

        // transition
        auto& layer = maLayers[tracker.layer_index];
        if (procent >= 1.f) { // ye, we reset all the stuff, because the async transition is too far
            layer.ease_func_pos = nullptr;
            layer.ease_func_alpha = nullptr;
            layer.ease_func_swap = nullptr;
            layer.change = { 0 };
            layer.ease_progress = { 0.f };
        }
        else {
            if (layer.change.pos) layer.ease_progress.pos = procent; // transition will happen in render 
            if (layer.change.alpha) {
                layer.ease_progress.alpha = procent;
                layer.alpha = lerp(layer.ease_func_alpha(procent), 0.f, 255.f);
            }
            if (layer.change.swap) layer.ease_progress.swap = procent; // same with this
        }
    }
}

void FilmLayerist::render() {
    // do there
}

void FilmLayerist::registerLayerKeypointAdd(FilmKeypoint* keypoint, LockerIndex kpllocal) {
    auto texind = ((FilmKeypointLayerAdd*)keypoint)->texind;
    Layer l;
    l.texind = texind;
    l.rect = pFilmScene->getTextureManager()->GetLockerTexture(pFilmScene->getTextureIndex(texind)).getRect();
    mKeypointPtrLocker[kpllocal].layer_index = maLayers.size();
    maLayers.push_back(l);
}

void FilmLayerist::registerLayerKeypointInteractPos(FilmKeypoint* keypoint, LayerIndex li) {
    auto kp = (FilmKeypointLayerInteractPos*)keypoint;
    auto& layer = maLayers[li];
    layer.rect_from = layer.rect;
    layer.rect = kp->pos;
    layer.use_from_manager = false;
    layer.ease_progress.pos = 0.f;
    layer.change.pos = ((layer.ease_func_pos = kp->ease_func) == nullptr);
}

void FilmLayerist::registerLayerKeypointInteractAlpha(FilmKeypoint* keypoint, LayerIndex li) {
    auto kp = (FilmKeypointLayerInteractAlpha*)keypoint;
    auto& layer = maLayers[li];
    layer.alpha = kp->alpha;
    layer.use_from_manager = false;
    layer.ease_progress.alpha = 0.f;
    layer.change.alpha = ((layer.ease_func_alpha = kp->ease_func) == nullptr);
}

float FilmLayerist::updateTimeProcenting(KeypointTracker& tracker) {
    auto& ttimer = tracker.timer;
    float procent = 0.f;
    bool is_procent_full = false;
    if (ttimer.frame_delay != -1) {
        if (ttimer.frame_delay > 0) {
            procent = ttimer.frame_delay / float(tracker.keypoint_ptr->frame_delay); // all operations happen there so it won't be x/0
            ttimer.frame_delay--;
        } else is_procent_full = true;
    } else {
        if (ttimer.delay.count() > 0) {
            procent = ttimer.delay.count() / float(tracker.keypoint_ptr->delay.count()); // same with this
            ttimer.delay -= std::chrono::duration_cast<std::chrono::milliseconds>(pClock->Now() - mPrev);
        } else is_procent_full = true;
    }
    return is_procent_full ? 1.f : 1.f - procent;
}

