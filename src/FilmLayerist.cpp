#include "FilmLayerist.h"

void FilmLayerist::registerLayerKeypoint(FilmKeypointLayer* keypoint) {
    assert(keypoint->type().specific_type >= FilmKeypointLayer::Add && keypoint->type().specific_type <= FilmKeypointLayer::Remove);
    assert(keypoint->layerindx < maLayers.size() || keypoint->type().specific_type == FilmKeypointLayer::Add);

    LayerIndex li = (keypoint->type().specific_type != FilmKeypointLayer::Add) ? keypoint->layerindx : -1;

    switch (keypoint->type().specific_type) { // it's mess now, but I must go further
    case FilmKeypointLayer::Add:             registerLayerKeypointAdd(dynamic_cast<FilmKeypointLayerAdd*>(keypoint));                                  break;
    case FilmKeypointLayer::InteractPos:     registerLayerKeypointInteractAnyPos(dynamic_cast<FilmKeypointLayerInteractRect*>(keypoint), li, Pos);     break;
    case FilmKeypointLayer::InteractRectPos: registerLayerKeypointInteractAnyPos(dynamic_cast<FilmKeypointLayerInteractRect*>(keypoint), li, RectPos); break;
    case FilmKeypointLayer::InteractPartPos: registerLayerKeypointInteractAnyPos(dynamic_cast<FilmKeypointLayerInteractRect*>(keypoint), li, PartPos); break;
    case FilmKeypointLayer::InteractAlpha:   registerLayerKeypointInteractAlpha(keypoint, li);                                           break;
    case FilmKeypointLayer::InteractDefaultPos: maLayers[li].rect.ease_tracker.setDefault();                                             break;
    case FilmKeypointLayer::InteractDefaultPartPos: maLayers[li].part.ease_tracker.setDefault();                                         break;
    case FilmKeypointLayer::InteractTransparentSwap:
    case FilmKeypointLayer::InteractSwap: _FALLTHROUGH
        registerLayerKeypointInteractSwap(keypoint, li);
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
    case FilmKeypointLayer::Await: assert(false); break; // useless?
    default: assert(false); break;
    }

}

FilmTimer FilmLayerist::getLongestWaiting() const {
    FilmTimer longest;
    auto iter = mKeypointPtrLocker.cbegin();
    for (iter; iter != mKeypointPtrLocker.cend(); iter++) {
        const auto& layer = maLayers[iter->layer_index];
        longest = FilmKP::max(longest, layer.alpha.ease_tracker.getLimiter());
        longest = FilmKP::max(longest, layer.rect.ease_tracker.getLimiter());
        longest = FilmKP::max(longest, layer.texind.ease_tracker.getLimiter());
        longest = FilmKP::max(longest, layer.part.ease_tracker.getLimiter());
    }
    
    return longest;
}

void FilmLayerist::update() {
    auto it = mKeypointPtrLocker.begin();
    while(it != mKeypointPtrLocker.end()) {
        auto& layer = maLayers[it->layer_index];
        it->update();

        if (it->is_finished()) {
            if (it->ease_texind_tr_ptr) // handle the swap logic
                finalizeSwap(it);
            it = mKeypointPtrLocker.popFromLocker(it);
        } else it++;
    }
}

void FilmLayerist::render() {
    bool tex_from = false;
    bool tex_to = false;
    SDL_FRect rect = { 0.f };
    SDL_FRect part = { 0.f };

    SDL_FRect* res_rect = nullptr;
    SDL_FRect* res_part = nullptr;

    uint8_t alpha = 0;

    for (auto& li : maActiveLayerIndexes) {
        auto& layer = maLayers[li];
        tex_from = layer.texind.elem_from != -1;
        tex_to = layer.texind.elem_to != -1;

        if (layer.rect.ease_tracker.isDefault()) res_rect = nullptr;
        else {
            if (layer.rect.ease_tracker.isProgress()) {
                rect = lerp_rect(layer.rect.elem_from, layer.rect.elem_to, layer.rect.ease_tracker);
                res_rect = &rect;
            } else res_rect = &(layer.rect.elem_to);
        }

        if (!layer.part.ease_tracker.isDefault()) {
            res_part = layer.part.ease_tracker.isProgress() ? &part : &(layer.part.elem_to);
        } else res_part = nullptr;

        if (layer.part.ease_tracker.isProgress())
            part = lerp_rect(layer.part.elem_from, layer.part.elem_to, layer.part.ease_tracker);

        if (layer.alpha.ease_tracker.isProgress())
            alpha = lerp(layer.alpha.ease_tracker, layer.alpha.elem_from, layer.alpha.elem_to);
        else alpha = layer.alpha.elem_to;

        if (layer.texind.ease_tracker.isProgress())
            renderSwapProgression(li, res_rect, res_part, alpha);
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
    l.rect.ease_tracker.reset();

    maLayers.push_back(l);
}

_MAYBE_UNUSED void FilmLayerist::registerLayerKeypointInteractAnyPos(FilmKeypointLayerInteractRect* keypoint, LayerIndex li, RegLayerKpInterAPosEnum enum_pos) {
    Layer::TransitionableParameter<SDL_FRect>* pos_ptr;
    pos_ptr = enum_pos == PartPos ? &(maLayers[li].part) : &(maLayers[li].rect);

    pos_ptr->shift_elem();
    if (enum_pos == Pos) {
        pos_ptr->elem_to.x = keypoint->rect.x;
        pos_ptr->elem_to.y = keypoint->rect.y;
    }
    else pos_ptr->elem_to = keypoint->rect;

    pos_ptr->ease_tracker.setEase(keypoint->ease_func);

    if (pos_ptr->ease_tracker.isEase()) {
        pos_ptr->ease_tracker.start(*dynamic_cast<FilmTimer*>(keypoint));
        registerTracker(keypoint, li, enum_pos == PartPos ? KeypointTracker::TrPart : KeypointTracker::TrRect);
    }
}

_MAYBE_UNUSED void FilmLayerist::registerLayerKeypointInteractAlpha(FilmKeypointLayer* keypoint, LayerIndex li) {
    auto kp = dynamic_cast<FilmKeypointLayerInteractAlpha*>(keypoint);
    maLayers[li].alpha.elem_to = kp->alpha;
    maLayers[li].alpha.ease_tracker.reset();
    maLayers[li].alpha.ease_tracker.setEase(kp->ease_func);
    maLayers[li].alpha.ease_tracker.start(*dynamic_cast<FilmTimer*>(keypoint));
    registerTracker(keypoint, li, KeypointTracker::TrAlpha);
}

_MAYBE_UNUSED void FilmLayerist::registerLayerKeypointInteractSwap(FilmKeypointLayer* keypoint, LayerIndex li) {
    const auto kpt_swap = (FilmKeypointLayerInteractSwap*)keypoint;
    FilmKeypointLayerSwap::SwapMode swapmode = kpt_swap->swap;

    if (keypoint->type().specific_type == FilmKeypointLayer::InteractTransparentSwap) {
        auto kpt = dynamic_cast<FilmKeypointLayerInteractTransparentSwap*>(keypoint);

        maLayers[li].texind.ease_tracker.setEase(kpt->ease_func);
        if (maLayers[li].texind.ease_tracker.isEase()) {
            maLayers[li].texind.ease_tracker.start(*dynamic_cast<FilmTimer*>(keypoint));
            registerTracker(keypoint, li, KeypointTracker::TrTexInd);
        }
    }
    else {
        switch (swapmode) {
        case FilmKeypointLayerSwap::KeepNotDeformed: {
            const auto tex = pTexMgr->GetLockerTexture(kpt_swap->texindx);
            maLayers[li].rect.elem_to = tex.getRectRes();
            maLayers[li].part.elem_to = tex.getRectPart();
        }   break;
        case FilmKeypointLayerSwap::SetDefault:
            maLayers[li].rect.ease_tracker.setDefault();
            maLayers[li].part.ease_tracker.setDefault();
            break;
        case FilmKeypointLayerSwap::NewTransform:
            if (kpt_swap->swap_rect_ptr)
                maLayers[li].rect.ease_tracker.setDefault();
            else maLayers[li].rect.elem_to = *kpt_swap->swap_rect_ptr;

            if (kpt_swap->swap_part_ptr)
                maLayers[li].part.ease_tracker.setDefault();
            else maLayers[li].part.elem_to = *kpt_swap->swap_part_ptr;
            break;
        }
    }

    maLayers[li].texind.shift_elem();
    maLayers[li].texind.elem_to = dynamic_cast<FilmKeypointLayerInteractSwap*>(keypoint)->texindx;

}

void FilmLayerist::finalizeSwap(LockerSimple<FilmLayerist::KeypointTracker>::Iterator iter) {
    const auto kpt_swap = dynamic_cast<FilmKeypointLayerInteractTransparentSwap*>((*iter).keypoint_ptr);
    const auto li = iter->layer_index;
    const auto swapmode = kpt_swap->swap;

    maLayers[li].rect.shift_elem();
    maLayers[li].part.shift_elem();

    switch (swapmode) {
    case FilmKeypointLayerSwap::KeepNotDeformed:
    {
        const auto tex = pTexMgr->GetLockerTexture(kpt_swap->texindx);
        maLayers[li].rect.elem_to = tex.getRectRes();
        maLayers[li].part.elem_to = tex.getRectPart();
    }   break;
    case FilmKeypointLayerSwap::SetDefault:
        maLayers[li].rect.ease_tracker.setDefault();
        maLayers[li].part.ease_tracker.setDefault();
        break;
    case FilmKeypointLayerSwap::NewTransform:
        if (kpt_swap->swap_rect_ptr)
            maLayers[li].rect.ease_tracker.setDefault();
        else maLayers[li].rect.elem_to = *kpt_swap->swap_rect_ptr;

        if (kpt_swap->swap_part_ptr)
            maLayers[li].part.ease_tracker.setDefault();
        else maLayers[li].part.elem_to = *kpt_swap->swap_part_ptr;
        break;
    }
}

void FilmLayerist::renderSwapProgression(LayerIndex li, SDL_FRect* res_rect, SDL_FRect* res_part, uint8_t alpha) {
    auto& layer = maLayers[li];

    const auto tex_from = layer.texind.elem_from != -1;
    const auto tex_to = layer.texind.elem_to != -1;

    if (tex_from && !tex_to) {
        pTexMgr->GetLockerTexture(layer.texind.elem_from).renderRaw(res_part, res_rect, 255 - alpha);
        return;
    }

    const auto tracked_kp = dynamic_cast<FilmKeypointLayerInteractTransparentSwap*>(mKeypointPtrLocker[layer.trackerind].keypoint_ptr);

    switch (tracked_kp->swap) {
    default:
    case FilmKeypointLayerSwap::KeepInAspect: _FALLTHROUGH
        if (tex_from) pTexMgr->GetLockerTexture(layer.texind.elem_from).renderRaw(res_part, res_rect, 255 - alpha);
        pTexMgr->GetLockerTexture(layer.texind.elem_to).renderRaw(res_part, res_rect, alpha);
        break;
    case FilmKeypointLayerSwap::KeepNotDeformed:
        if (tex_from) pTexMgr->GetLockerTexture(layer.texind.elem_from).renderRaw(res_part, res_rect, 255 - alpha);
        pTexMgr->GetLockerTexture(layer.texind.elem_to).render();
        break;
    case FilmKeypointLayerSwap::SetDefault:
        if (tex_from) pTexMgr->GetLockerTexture(layer.texind.elem_from).renderRaw(res_part, res_rect, 255 - alpha);
        pTexMgr->GetLockerTexture(layer.texind.elem_to).renderRaw(nullptr, nullptr, alpha);
        break;
    case FilmKeypointLayerSwap::NewTransform:
        if (tracked_kp->swap_rect_ptr)
            maLayers[li].rect.ease_tracker.setDefault();
        else maLayers[li].rect.elem_to = *tracked_kp->swap_rect_ptr;

        if (tracked_kp->swap_part_ptr)
            maLayers[li].part.ease_tracker.setDefault();
        else maLayers[li].part.elem_to = *tracked_kp->swap_part_ptr;
        break;
    }
}

void FilmLayerist::registerTracker(FilmKeypointLayer* keypoint, LayerIndex li, char tracker_affect_mask) {
    if (keypoint->is_zero()) return;
    
    KeypointTracker tracker;
    tracker.keypoint_ptr = keypoint;
    tracker.layer_index = li;

    if (tracker_affect_mask & KeypointTracker::TrRect)
        tracker.ease_rect_tr_ptr = &maLayers[li].rect.ease_tracker;

    if (tracker_affect_mask & KeypointTracker::TrPart)
        tracker.ease_part_tr_ptr = &maLayers[li].part.ease_tracker;

    if (tracker_affect_mask & KeypointTracker::TrAlpha)
        tracker.ease_alpha_tr_ptr = &maLayers[li].alpha.ease_tracker;

    if (tracker_affect_mask & KeypointTracker::TrTexInd)
        tracker.ease_texind_tr_ptr = &maLayers[li].texind.ease_tracker;

    auto indx = mKeypointPtrLocker.pushInLocker(std::move(tracker));
    assert(li >= 0 && li < maLayers.size());
    maLayers[li].trackerind = indx;
}

