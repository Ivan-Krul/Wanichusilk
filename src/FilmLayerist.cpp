#include "FilmLayerist.h"

void FilmLayerist::registerLayerKeypoint(FilmKeypoint* keypoint) {
    assert(keypoint->type() >= FilmKeypointType::LayerAdd && keypoint->type() <= FilmKeypointType::LayerRemove);

    KeypointTracker kt = KeypointTracker{ keypoint, *keypoint };

    LayerIndex li = (keypoint->type() != FilmKeypointType::LayerAdd) ? ((FilmKeypointLayer*)keypoint)->layerindx : -1;

    switch (keypoint->type()) { // it's mess now, but I must go further
    case FilmKeypointType::LayerAdd:             registerLayerKeypointAdd((FilmKeypointLayerAdd*)keypoint, kpllocal);                                break;
    case FilmKeypointType::LayerInteractPos:     registerLayerKeypointInteractAnyPos((FilmKeypointLayerInteractRect*)keypoint, maLayers[li], false); break;
    case FilmKeypointType::LayerInteractPartPos: registerLayerKeypointInteractAnyPos((FilmKeypointLayerInteractRect*)keypoint, maLayers[li], true);  break;
    case FilmKeypointType::LayerInteractAlpha:   registerLayerKeypointInteractAlpha(keypoint, li);                                                   break;
    case FilmKeypointType::LayerInteractDefaultPos:
        maLayers[li].usage.rect = false;
        break;
    case FilmKeypointType::LayerInteractDefaultPartPos:
        maLayers[li].usage.part = false;
        break;
    case FilmKeypointType::LayerInteractTransparentSwap:
        maLayers[li].ease_progress.swap = 0.f;
        maLayers[li].change.swap = ((maLayers[li].ease_func_swap = ((FilmKeypointEase*)keypoint)->ease_func) == nullptr);
    case FilmKeypointType::LayerInteractSwap: _FALLTHROUGH
        maLayers[li].texind_from = maLayers[li].texind;
        maLayers[li].texind = ((FilmKeypointLayerInteractSwap*)keypoint)->texindx;
        maLayers[li].rect = pTexMgr->GetLockerTexture(maLayers[li].texind).getRectRes();
        maLayers[li].use_from_manager = true;
        break;
    case FilmKeypointType::LayerEnable:
        maActiveLayerIndexes.push_back(li);
        break;
    case FilmKeypointType::LayerDisable:
    {
        auto iter = std::find(maActiveLayerIndexes.begin(), maActiveLayerIndexes.end(), li);
        if(iter != maActiveLayerIndexes.end()) maActiveLayerIndexes.erase(iter);
    }   break;
    case FilmKeypointType::LayerInteractDefault:
        maLayers[li].use_from_manager = true;
        break;
    case FilmKeypointType::LayerRemove:
    {
        maLayers.erase(maLayers.begin() + li);
        auto iter = std::find(maActiveLayerIndexes.begin(), maActiveLayerIndexes.end(), li);
        maActiveLayerIndexes.erase(iter);
        mKeypointPtrLocker.popFromLocker(li);
    }   break;
    case FilmKeypointType::LayerAwait: assert(false); break;
    default: assert(false); break;
    }

}

FilmTimer FilmLayerist::getLongestWaiting() const {
    FilmTimer longest;
    auto iter = mKeypointPtrLocker.cbegin();
    for (iter; iter != mKeypointPtrLocker.cend(); iter++) {
        if (iter->timer.delay > longest.delay)
            longest.delay = iter->timer.delay;
        if (iter->timer.frame_delay > longest.frame_delay)
            longest.frame_delay = iter->timer.frame_delay;
    }
    return longest;
}

void FilmLayerist::update() {
    for (auto& tracker : mKeypointPtrLocker) { // questionable decision to handle concurrency and/or transfer between states
        float procent = updateTimeProcenting(tracker);
        SDL_Log("t: %f", procent);

        // transition
        auto& layer = maLayers[tracker.layer_index];
        if (!tracker.timer.need_await && procent >= 1.f) { // ye, we reset all the stuff, because the async transition is too far
            layer.ease_func_rect = nullptr;
            layer.ease_func_part = nullptr;
            layer.ease_func_alpha = nullptr;
            layer.ease_func_swap = nullptr;
            layer.change = { 0 };
            layer.ease_progress = { 0.f };
            layer.texind_from = -1;
            
        }
        else {
            if (layer.change.rect) layer.ease_progress.rect = procent; // transition will happen in render 
            if (layer.change.alpha) {
                layer.ease_progress.alpha = procent;
                if(layer.ease_func_alpha)
                layer.alpha = lerp(layer.ease_func_alpha(procent), 0.f, 255.f);
            }
            if (layer.change.swap) layer.ease_progress.swap = procent; // same with this
            if (layer.change.part) layer.ease_progress.part = procent; // and this too
        }
    }
}

void FilmLayerist::render() {
    bool tex_from = false;
    float progress = 0.f;
    SDL_FRect rect = { 0.f };
    SDL_FRect part = { 0.f };

    SDL_FRect* res_rect = nullptr;
    SDL_FRect* res_part = nullptr;

    for (auto& layer : maLayers) {
        tex_from = layer.texind_from != -1;

        res_rect = layer.usage.rect ? (layer.change.rect ? &rect : &layer.rect) : nullptr;
        if (layer.change.rect) {
            progress = layer.ease_func_rect(layer.ease_progress.rect);
            rect = lerpRect(layer.rect_from, layer.rect, progress);
        }

        res_part = layer.usage.part ? (layer.change.part ? &part : &layer.part) : nullptr;
        if (layer.change.part) {
            progress = layer.ease_func_part(layer.ease_progress.part);
            part = lerpRect(layer.part_from, layer.part, progress);
        }

        if (layer.change.swap) {
            progress = layer.ease_func_rect(layer.ease_progress.rect);
            if (tex_from)
                pTexMgr->GetLockerTexture(layer.texind_from).renderRaw(res_part, res_rect, (1 - progress) * layer.alpha);

            pTexMgr->GetLockerTexture(layer.texind).renderRaw(res_part, res_rect, progress * layer.alpha);
        } else if (layer.use_from_manager)
            pTexMgr->GetLockerTexture(layer.texind).render();
        else
            pTexMgr->GetLockerTexture(layer.texind).renderRaw(res_part, res_rect, layer.alpha);
        
    }
}

void FilmLayerist::registerLayerKeypointAdd(FilmKeypointLayerAdd* keypoint, LockerIndex kpllocal) {
    Layer l;
    l.texind = keypoint->texind;
    l.rect = pTexMgr->GetLockerTexture(keypoint->texind).getRectRes();
    l.usage.rect = true;
    maLayers.push_back(l);
}

void FilmLayerist::registerLayerKeypointInteractAnyPos(FilmKeypointLayerInteractRect* keypoint, Layer& layer, bool is_src) {
    layer.use_from_manager = false;

    if (is_src) {
        layer.usage.part = true;
        layer.ease_progress.part = 0.f;
        layer.part_from = layer.part;
        layer.part = keypoint->rect;
        layer.change.part = ((layer.ease_func_part = keypoint->ease_func) == nullptr);
    } else {
        layer.usage.rect = true;
        layer.ease_progress.rect = 0.f;
        layer.rect_from = layer.rect;
        layer.rect = keypoint->rect;
        layer.change.rect = ((layer.ease_func_rect = keypoint->ease_func) == nullptr);
    }
}

void FilmLayerist::registerLayerKeypointInteractAlpha(FilmKeypoint* keypoint, LayerIndex li) {
    auto kp = (FilmKeypointLayerInteractAlpha*)keypoint;
    auto& layer = maLayers[li];
    layer.alpha = kp->alpha;
    layer.use_from_manager = false;
    layer.ease_progress.alpha = 0.f;
    layer.change.alpha = ((layer.ease_func_alpha = kp->ease_func) != nullptr);
}

float FilmLayerist::updateTimeProcenting(KeypointTracker& tracker) {
    auto& ttimer = tracker.timer;
    float procent = 0.f;
    if (ttimer.is_zero()) return 1.f;

    if (!ttimer.need_time_delay) {
        procent = ttimer.frame_delay / float(tracker.keypoint_ptr->frame_delay); // all operations happen there so it won't be x/0
        ttimer.frame_delay--;
    } else {
        procent = ttimer.delay.count() / float(tracker.keypoint_ptr->delay.count()); // same with this
        ttimer.delay -= std::chrono::duration_cast<std::chrono::milliseconds>(pClock->Now() - mPrev);
    }
    return 1.f - procent;
}

SDL_FRect FilmLayerist::lerpRect(const SDL_FRect& from, const SDL_FRect& to, float t) {
    SDL_FRect rect;
    rect.x = lerp(t, from.x, to.x);
    rect.y = lerp(t, from.y, to.y);
    rect.w = lerp(t, from.w, to.w);
    rect.h = lerp(t, from.h, to.h);
    return rect;
}

