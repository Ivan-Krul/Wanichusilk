#include "FilmLayerist.h"

void FilmLayerist::registerLayerKeypoint(FilmKeypoint* keypoint) {
    assert(keypoint->type().specific_type >= FilmKeypointLayer::Add && keypoint->type().specific_type <= FilmKeypointLayer::Remove);

    LayerIndex li = (keypoint->type().specific_type != FilmKeypointLayer::Add) ? ((FilmKeypointLayer*)keypoint)->layerindx : -1;

    switch (keypoint->type().specific_type) { // it's mess now, but I must go further
    case FilmKeypointLayer::Add:             registerLayerKeypointAdd((FilmKeypointLayerAdd*)keypoint);                                break;
    case FilmKeypointLayer::InteractPos:     registerLayerKeypointInteractAnyPos((FilmKeypointLayerInteractRect*)keypoint, li, false); break;
    case FilmKeypointLayer::InteractPartPos: registerLayerKeypointInteractAnyPos((FilmKeypointLayerInteractRect*)keypoint, li, true);  break;
    case FilmKeypointLayer::InteractAlpha:   registerLayerKeypointInteractAlpha(keypoint, li);                                         break;
    case FilmKeypointLayer::InteractDefaultPos: maLayers[li].rect.ease_tracker.setDefault(); break;
    case FilmKeypointLayer::InteractDefaultPartPos: maLayers[li].part.ease_tracker.setDefault(); break;
    case FilmKeypointLayer::InteractTransparentSwap:
        maLayers[li].texind.ease_tracker.setEase(((FilmKeypointEase*)keypoint)->ease_func);
        if (maLayers[li].texind.ease_tracker.isEase()) {
            maLayers[li].texind.ease_tracker.start(*((FilmTimer*)keypoint));
            registerTracker(keypoint, li);
        }
    case FilmKeypointLayer::InteractSwap: _FALLTHROUGH
        maLayers[li].texind.elem_from = maLayers[li].texind.elem_to;
        maLayers[li].texind.elem_to = ((FilmKeypointLayerInteractSwap*)keypoint)->texindx;
        maLayers[li].rect.elem_to = pTexMgr->GetLockerTexture(maLayers[li].texind.elem_to).getRectRes();
        break;
    case FilmKeypointLayer::Enable: maActiveLayerIndexes.push_back(li); break;
    case FilmKeypointLayer::Disable:
    {
        auto iter = std::find(maActiveLayerIndexes.begin(), maActiveLayerIndexes.end(), li);
        if(iter != maActiveLayerIndexes.end()) maActiveLayerIndexes.erase(iter);
    }   break;
    case FilmKeypointLayer::InteractDefault: maLayers[li].set_to_default(); break;
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
        auto& layer = maLayers[iter->layer_index];
        longest.delay = std::max(longest.delay, layer.alpha.ease_tracker.getLimiter().delay);
        longest.delay = std::max(longest.delay, layer.rect.ease_tracker.getLimiter().delay);
        longest.delay = std::max(longest.delay, layer.part.ease_tracker.getLimiter().delay);
        longest.delay = std::max(longest.delay, layer.texind.ease_tracker.getLimiter().delay);

        longest.frame_delay = std::max(longest.frame_delay, layer.alpha.ease_tracker.getLimiter().frame_delay);
        longest.frame_delay = std::max(longest.frame_delay, layer.rect.ease_tracker.getLimiter().frame_delay);
        longest.frame_delay = std::max(longest.frame_delay, layer.part.ease_tracker.getLimiter().frame_delay);
        longest.frame_delay = std::max(longest.frame_delay, layer.texind.ease_tracker.getLimiter().frame_delay);
    }
    return longest;
}

void FilmLayerist::update() {
    bool accu_no_progress;
    auto it = mKeypointPtrLocker.begin();
    while(it != mKeypointPtrLocker.end()) {
        auto& tracker = *it;
        accu_no_progress = false;
        auto& layer = maLayers[tracker.layer_index]; // transition
        if (layer.rect.ease_tracker.isProgress()) { 
            layer.rect.ease_tracker.update();
            accu_no_progress = true;
        }
        if (layer.part.ease_tracker.isProgress()) {
            layer.part.ease_tracker.update();
            accu_no_progress = true;
        }
        if (layer.texind.ease_tracker.isProgress()) {
            layer.texind.ease_tracker.update();
            accu_no_progress = true;
        }
        if (layer.alpha.ease_tracker.isProgress()) {
            layer.alpha.ease_tracker.update();
            accu_no_progress = true;
        }

        if (!accu_no_progress) {
            it = mKeypointPtrLocker.popFromLocker(it);
        } else it++;
    }
}

void FilmLayerist::render() {
    bool tex_from = false;
    SDL_FRect rect = { 0.f };
    SDL_FRect part = { 0.f };

    SDL_FRect* res_rect = nullptr;
    SDL_FRect* res_part = nullptr;

    uint8_t alpha = 0;

    for (auto& li : maActiveLayerIndexes) {
        auto& layer = maLayers[li];
        tex_from = layer.texind.elem_from != -1;

        if (layer.rect.ease_tracker.isDefault()) {
            if (layer.rect.ease_tracker.isProgress())
                res_rect = &rect;
            else
                res_rect = &(layer.rect.elem_to);
        } else res_rect = nullptr;

        if (layer.rect.ease_tracker.isProgress())
            rect = lerpRect(layer.rect.elem_from, layer.rect.elem_to, layer.rect.ease_tracker);

        res_part = !(layer.part.ease_tracker.isDefault()) ? (layer.part.ease_tracker.isProgress() ? &part : &layer.part.elem_to) : nullptr;
        if (layer.part.ease_tracker.isProgress())
            part = lerpRect(layer.part.elem_from, layer.part.elem_to, layer.part.ease_tracker);

        if (layer.alpha.ease_tracker.isProgress())
            alpha = lerp(layer.alpha.ease_tracker, layer.alpha.elem_from, layer.alpha.elem_to);
        else alpha = layer.alpha.elem_to;

        if (layer.texind.ease_tracker.isProgress()) {
            if (tex_from)
                pTexMgr->GetLockerTexture(layer.texind.elem_from).renderRaw(res_part, res_rect, (1 - layer.texind.ease_tracker) * layer.alpha.elem_from);

            pTexMgr->GetLockerTexture(layer.texind.elem_to).renderRaw(res_part, res_rect, layer.texind.ease_tracker * layer.alpha.elem_to);
        }
        else if (layer.is_default())
            pTexMgr->GetLockerTexture(layer.texind.elem_to).render();
        else
            pTexMgr->GetLockerTexture(layer.texind.elem_to).renderRaw(res_part, res_rect, alpha);
    }
}

void FilmLayerist::registerLayerKeypointAdd(FilmKeypointLayerAdd* keypoint) {
    Layer l;
    l.texind.elem_to = keypoint->texind;
    l.rect.elem_to = pTexMgr->GetLockerTexture(keypoint->texind).getRectRes();

    l.rect.ease_tracker.setClock(pClock);
    l.part.ease_tracker.setClock(pClock);
    l.texind.ease_tracker.setClock(pClock);
    l.alpha.ease_tracker.setClock(pClock);

    l.texind.ease_tracker.reset();

    maLayers.push_back(l);
}

void FilmLayerist::registerLayerKeypointInteractAnyPos(FilmKeypointLayerInteractRect* keypoint, LayerIndex li, bool is_src) {
    Layer::TransitionableParameter<SDL_FRect>* pos_ptr;
    pos_ptr = is_src ? &(maLayers[li].part) : &(maLayers[li].rect);

    pos_ptr->shift_elem();
    pos_ptr->elem_to = keypoint->rect;
    pos_ptr->ease_tracker.setEase(keypoint->ease_func);

    if (pos_ptr->ease_tracker.isEase()) {
        pos_ptr->ease_tracker.start(*((FilmTimer*)keypoint));
        registerTracker(keypoint, li);
    }
}

void FilmLayerist::registerLayerKeypointInteractAlpha(FilmKeypoint* keypoint, LayerIndex li) {
    auto kp = (FilmKeypointLayerInteractAlpha*)keypoint;
    maLayers[li].alpha.elem_to = kp->alpha;
    maLayers[li].alpha.ease_tracker.reset();
    maLayers[li].alpha.ease_tracker.setEase(kp->ease_func);
    maLayers[li].alpha.ease_tracker.start(*((FilmTimer*)keypoint));
    registerTracker(keypoint, li);
}
/*
float FilmLayerist::updateTimeProcenting(KeypointTracker& tracker) {
    auto& ttimer = tracker.timer;
    float procent = 0.f;
    if (ttimer.is_zero()) return 1.f;

    if (!ttimer.need_time_delay) {
        procent = ttimer.frame_delay / float(tracker.keypoint_ptr->frame_delay); // all operations happen there so it won't be x/0
        ttimer.frame_delay--;
    } else {
        procent = ttimer.delay.count() / float(tracker.keypoint_ptr->delay.count()); // same with this
        ttimer.delay -= pClock->DeltaTime();
    }
    return 1.f - procent;
}
*/
void FilmLayerist::registerTracker(FilmKeypoint* keypoint, LayerIndex li) {
    auto indx = mKeypointPtrLocker.pushInLocker(KeypointTracker{ keypoint, li });
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

