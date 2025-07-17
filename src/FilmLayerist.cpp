#include "FilmLayerist.h"

void FilmLayerist::registerLayerKeypoint(FilmKeypoint* keypoint) {
    assert(keypoint->type().specific_type >= FilmKeypointLayer::Add && keypoint->type().specific_type <= FilmKeypointLayer::Remove);

    LayerIndex li = (keypoint->type().specific_type != FilmKeypointLayer::Add) ? ((FilmKeypointLayer*)keypoint)->layerindx : -1;

    float(*ease)(float) = nullptr;

    switch (keypoint->type().specific_type) { // it's mess now, but I must go further
    case FilmKeypointLayer::Add:             registerLayerKeypointAdd((FilmKeypointLayerAdd*)keypoint);                                break;
    case FilmKeypointLayer::InteractPos:     registerLayerKeypointInteractAnyPos((FilmKeypointLayerInteractRect*)keypoint, li, false); break;
    case FilmKeypointLayer::InteractPartPos: registerLayerKeypointInteractAnyPos((FilmKeypointLayerInteractRect*)keypoint, li, true);  break;
    case FilmKeypointLayer::InteractAlpha:   registerLayerKeypointInteractAlpha(keypoint, li);                                         break;
    case FilmKeypointLayer::InteractDefaultPos:
        maLayers[li].rect = Layer::c_ease_use_default;
        break;
    case FilmKeypointLayer::InteractDefaultPartPos:
        maLayers[li].part = Layer::c_ease_use_default;
        break;
    case FilmKeypointLayer::InteractTransparentSwap:
        maLayers[li].texind.set_ease(((FilmKeypointEase*)(keypoint))->ease_func);
        if(maLayers[li].texind.is_easing()) registerTracker(keypoint, li);
    case FilmKeypointLayer::InteractSwap: _FALLTHROUGH
        maLayers[li].texind.elem_from = maLayers[li].texind.elem_to;
        maLayers[li].texind.elem_to = ((FilmKeypointLayerInteractSwap*)keypoint)->texindx;
        maLayers[li].rect.elem_to = pTexMgr->GetLockerTexture(maLayers[li].texind.elem_to).getRectRes();
        break;
    case FilmKeypointLayer::Enable:
        maActiveLayerIndexes.push_back(li);
        break;
    case FilmKeypointLayer::Disable:
    {
        auto iter = std::find(maActiveLayerIndexes.begin(), maActiveLayerIndexes.end(), li);
        if(iter != maActiveLayerIndexes.end()) maActiveLayerIndexes.erase(iter);
    }   break;
    case FilmKeypointLayer::InteractDefault:
        maLayers[li].set_to_default();
        break;
    case FilmKeypointLayer::Remove:
    {
        maLayers.erase(maLayers.begin() + li);
        auto iter = std::find(maActiveLayerIndexes.begin(), maActiveLayerIndexes.end(), li);
        maActiveLayerIndexes.erase(iter);
        mKeypointPtrLocker.popFromLocker(li);
    }   break;
    case FilmKeypointLayer::Await: assert(false); break;
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
        if (procent >= 1.f) { // ye, we reset all the stuff, because the async transition is too far
            layer.alpha = Layer::c_ease_no_progress;
            layer.rect = Layer::c_ease_no_progress;
            layer.part = Layer::c_ease_no_progress;
            layer.texind = Layer::c_ease_no_progress;
            layer.texind.elem_from = -1;
            mKeypointPtrLocker.popFromLocker(tracker.index);
        }
        else {
            if (layer.rect.is_easing()) layer.rect = procent; // transition will happen in render 
            if (layer.alpha.is_easing()) {
                layer.alpha = procent;
                layer.alpha.elem_to = lerp(layer.alpha(procent), 0.f, 255.f);
            }
            if (layer.texind.is_easing()) layer.texind = procent; // same with this
            if (layer.part.is_easing()) layer.part = procent; // and this too
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
        tex_from = layer.texind.elem_from != -1;

        res_rect = layer.rect.is_default() ? (layer.rect.is_easing() ? &rect : &layer.rect.elem_to) : nullptr;
        if (layer.rect.is_easing()) {
            progress = layer.rect.in_ease();
            rect = lerpRect(layer.rect.elem_from, layer.rect.elem_to, progress);
        }

        res_part = layer.part.is_default() ? (layer.part.is_easing() ? &part : &layer.part.elem_to) : nullptr;
        if (layer.part.is_easing()) {
            progress = layer.part.in_ease();
            part = lerpRect(layer.part.elem_from, layer.part.elem_to, progress);
        }

        if (layer.texind.is_easing()) {
            progress = layer.texind.in_ease();
            if (tex_from)
                pTexMgr->GetLockerTexture(layer.texind.elem_from).renderRaw(res_part, res_rect, (1 - progress) * layer.alpha.elem_to);

            pTexMgr->GetLockerTexture(layer.texind).renderRaw(res_part, res_rect, progress * layer.alpha.elem_to);
        } else if (layer.is_default())
            pTexMgr->GetLockerTexture(layer.texind).render();
        else
            pTexMgr->GetLockerTexture(layer.texind).renderRaw(res_part, res_rect, layer.alpha.elem_to);
        
    }
}

void FilmLayerist::registerLayerKeypointAdd(FilmKeypointLayerAdd* keypoint) {
    Layer l;
    l.texind.elem_to = keypoint->texind;
    l.texind = Layer::c_ease_no_progress;
    l.rect.elem_to = pTexMgr->GetLockerTexture(keypoint->texind).getRectRes();
    maLayers.push_back(l);
}

void FilmLayerist::registerLayerKeypointInteractAnyPos(FilmKeypointLayerInteractRect* keypoint, LayerIndex li, bool is_src) {
    Layer::TransitionableParameter<SDL_FRect>* pos_ptr;
    pos_ptr = is_src ? &(maLayers[li].part) : &(maLayers[li].rect);

    (*pos_ptr) = Layer::c_ease_use_default;
    pos_ptr->shift_elem();
    pos_ptr->elem_to = keypoint->rect;
    pos_ptr->set_ease(keypoint->ease_func);
    if (maLayers[li].texind.is_easing()) registerTracker(keypoint, li);
}

void FilmLayerist::registerLayerKeypointInteractAlpha(FilmKeypoint* keypoint, LayerIndex li) {
    auto kp = (FilmKeypointLayerInteractAlpha*)keypoint;
    maLayers[li].alpha.elem_to = kp->alpha;
    maLayers[li].alpha = Layer::c_ease_no_progress;
    maLayers[li].alpha.set_ease(kp->ease_func);
    if (maLayers[li].alpha.is_easing()) registerTracker(keypoint, li);
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

void FilmLayerist::registerTracker(FilmKeypoint* keypoint, LayerIndex li) {
    auto indx = mKeypointPtrLocker.pushInLocker(KeypointTracker{ keypoint, *keypoint, li });
    mKeypointPtrLocker[indx].index = indx;
}

SDL_FRect FilmLayerist::lerpRect(const SDL_FRect& from, const SDL_FRect& to, float t) {
    SDL_FRect rect;
    rect.x = lerp(t, from.x, to.x);
    rect.y = lerp(t, from.y, to.y);
    rect.w = lerp(t, from.w, to.w);
    rect.h = lerp(t, from.h, to.h);
    return rect;
}

