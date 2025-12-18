#include "FilmLayerTexture.h"

FilmLayerTexture::FilmLayerTexture(Clock* clock, TextureManager* texmgr, LockerIndex texind) {
    pTexMgr = texmgr;
    setClock(clock);

    mTexInd.elem_to = texind;
    mRect.elem_to = pTexMgr->GetLockerTexture(texind).getRectRes();

    mPart.ease_tracker.setClock(clock);
    mRect.ease_tracker.setClock(clock);
    mAlpha.ease_tracker.setClock(clock);
    mTexInd.ease_tracker.setClock(clock);

    mRect.reset_tracker();
    mTexInd.reset_tracker();
}

void FilmLayerTexture::update() {
    if (maEases.isEmpty()) return;
    mPart.ease_tracker.update();
    mRect.ease_tracker.update();
    mAlpha.ease_tracker.update();
    mTexInd.ease_tracker.update();

    auto it = maEases.begin();
    while (it != maEases.end()) {
        if (it->ease->isEnded()) {
            if (it->ease == &(mTexInd.ease_tracker)) // handle the swap logic
                finalizeSwap(it);
            it = maEases.popFromLocker(it);
        } else it++;
    }
}

void FilmLayerTexture::render() const {
    SDL_FRect rect = { 0.f };
    SDL_FRect part = { 0.f };

    const SDL_FRect* res_rect = nullptr;
    const SDL_FRect* res_part = nullptr;

    uint8_t alpha = 0;

    if (mRect.is_default()) res_rect = nullptr;
    else {
        if (mRect.is_progress()) {
            rect = lerp_rect(mRect.elem_from, mRect.elem_to, mRect.ease_tracker);
            res_rect = &rect;
        } else res_rect = &(mRect.elem_to);
    }

    if (mPart.is_default()) res_part = nullptr;
    else {
        if (mPart.is_progress()) {
            part = lerp_rect(mPart.elem_from, mPart.elem_to, mPart.ease_tracker);
            res_part = &part;
        } else res_part = &(mPart.elem_to);
    }

    if (mAlpha.is_progress())
        alpha = lerp(mAlpha.ease_tracker, mAlpha.elem_from, mAlpha.elem_to);
    else alpha = mAlpha.elem_to;

    if (mTexInd.is_progress())
        renderSwap(res_rect, res_part, alpha);
    else if (mTexInd.elem_to == -1) return;
    else if (areAllTransitParamDefault())
        pTexMgr->GetLockerTexture(mTexInd.elem_to).render();
    else
        pTexMgr->GetLockerTexture(mTexInd.elem_to).renderRaw(res_part, res_rect, alpha);
}

inline void FilmLayerTexture::clear() {
    maEases.clear();
    mPart.clear();
    mRect.clear();
    mAlpha.clear();
    mTexInd.clear();
}

inline bool FilmLayerTexture::isWaiting() const noexcept {
    return maEases.isEmpty();
}

inline TimerStep FilmLayerTexture::getLongestWaiting() const noexcept {
    TimerStep longest = ClockFunc::max(mPart.ease_tracker.getLimiter(), mRect.ease_tracker.getLimiter());
    longest = ClockFunc::max(mAlpha.ease_tracker.getLimiter(), longest);
    longest = ClockFunc::max(mTexInd.ease_tracker.getLimiter(), longest);
    return longest;
}

void FilmLayerTexture::pushPosSetter(const FilmKeypointLayerInteractRect* keypoint, PosChangeEnum change) {
    TransitParam<SDL_FRect>* pos_ptr = change == PartPos ? &mPart : &mRect;

    pos_ptr->shift_elem();
    if (change == Pos) {
        pos_ptr->elem_to.x = keypoint->rect.x;
        pos_ptr->elem_to.y = keypoint->rect.y;
    } else pos_ptr->elem_to = keypoint->rect;
    pos_ptr->reset_tracker();
}

void FilmLayerTexture::pushTexIndSetter(FilmKeypointLayerInteractSwap* keypoint) {
    FilmKeypointLayerSwap::SwapMode swapmode = keypoint->swap;

    mRect.shift_elem();
    mPart.shift_elem();

    mTexInd.reset_tracker();
    mTexInd.shift_elem();
    mTexInd.elem_to = keypoint->texindx;

    switch (swapmode) {
    case FilmKeypointLayerSwap::KeepNotDeformed:
    {
        const auto& tex = pTexMgr->GetLockerTexture(mTexInd.elem_to);
        mRect.elem_to = tex.getRectRes();
        mPart.elem_to = tex.getRectPart();
    }   break;
    case FilmKeypointLayerSwap::SetDefault:
        mRect.set_default();
        mPart.set_default();
        break;
    case FilmKeypointLayerSwap::NewTransform: _FALLTHROUGH
        if (!keypoint->swap_rect_ptr) {
            mRect.elem_to = *keypoint->swap_rect_ptr;
            mRect.reset_tracker();
        } else
            mRect.set_default();

        if (keypoint->swap_part_ptr) {
            mPart.elem_to = *keypoint->swap_part_ptr;
            mPart.reset_tracker();
        } else
            mPart.set_default();
        break;
    }
}

bool FilmLayerTexture::onPushSetter(FilmKeypointLayer* keypoint) {
    switch (keypoint->type().specific_type) {
    case FilmKeypointLayer::InteractPos:     pushPosSetter(dynamic_cast<FilmKeypointLayerInteractRect*>(keypoint), Pos);     break;
    case FilmKeypointLayer::InteractRectPos: pushPosSetter(dynamic_cast<FilmKeypointLayerInteractRect*>(keypoint), RectPos); break;
    case FilmKeypointLayer::InteractPartPos: pushPosSetter(dynamic_cast<FilmKeypointLayerInteractRect*>(keypoint), PartPos); break;
    case FilmKeypointLayer::InteractAlpha:
    {
        const auto kp_alpha = dynamic_cast<FilmKeypointLayerInteractAlpha*>(keypoint);
        mAlpha.shift_elem();
        mAlpha.ease_tracker.reset();
        mAlpha.elem_to = kp_alpha->alpha;
    }   break;
    case FilmKeypointLayer::InteractDefaultPos:     mRect.set_default(); break;
    case FilmKeypointLayer::InteractDefaultPartPos: mPart.set_default(); break;
    case FilmKeypointLayer::InteractSwap:
        pushTexIndSetter(dynamic_cast<FilmKeypointLayerInteractSwap*>(keypoint));
        break;
    default:
        return true;
    }

    return false;
}

void FilmLayerTexture::pushPosTracker(const LockerIndex ease_indx, PosChangeEnum change) {
    auto& tracker = maEases[ease_indx];
    const auto keypoint = dynamic_cast<FilmKeypointLayerInteractRect*>(tracker.keypoint);
    TransitParam<SDL_FRect>* pos_ptr = change == PartPos ? &mPart : &mRect;
    pos_ptr->shift_elem();
    pos_ptr->ease_tracker.setEase(keypoint->ease_func);
    pos_ptr->ease_tracker.start(*dynamic_cast<TimerStep*>(keypoint));

    if (change == Pos) {
        pos_ptr->elem_to.x = keypoint->rect.x;
        pos_ptr->elem_to.y = keypoint->rect.y;
    } else pos_ptr->elem_to = keypoint->rect;

    tracker.ease = &(pos_ptr->ease_tracker);
}

bool FilmLayerTexture::onPushTracker(LockerIndex ease_indx) {
    auto& tracker = maEases[ease_indx];
    const auto keypoint = tracker.keypoint;
    switch (keypoint->type().specific_type) {
    case FilmKeypointLayer::InteractPos:     pushPosTracker(ease_indx, Pos);     break;
    case FilmKeypointLayer::InteractRectPos: pushPosTracker(ease_indx, RectPos); break;
    case FilmKeypointLayer::InteractPartPos: pushPosTracker(ease_indx, PartPos); break;
    case FilmKeypointLayer::InteractAlpha:
    {
        const auto kp_alpha = dynamic_cast<FilmKeypointLayerInteractAlpha*>(keypoint);
        mAlpha.shift_elem();
        mAlpha.ease_tracker.setEase(kp_alpha->ease_func);
        mAlpha.ease_tracker.start(*dynamic_cast<TimerStep*>(keypoint));
        mAlpha.elem_to = kp_alpha->alpha;
        tracker.ease = &(mAlpha.ease_tracker);
    }   break;
    case FilmKeypointLayer::InteractTransparentSwap:
    {
        const auto kp_swap = dynamic_cast<FilmKeypointLayerInteractTransparentSwap*>(keypoint);
        mTexInd.shift_elem();
        mTexInd.ease_tracker.setEase(kp_swap->ease_func);
        mTexInd.ease_tracker.start(*dynamic_cast<TimerStep*>(keypoint));
        mTexInd.elem_to = kp_swap->texindx;
        mTexInd.unused_padding = ease_indx;
        tracker.ease = &(mTexInd.ease_tracker);
    }   break;
    default:
        return true;
    }

    return false;
}

void FilmLayerTexture::renderSwap(const SDL_FRect* res_rect, const SDL_FRect* res_part, uint8_t max_alpha) const {
    const float progress = mTexInd.ease_tracker;
    if (mTexInd.elem_from != -1) pTexMgr->GetLockerTexture(mTexInd.elem_from).renderRaw(res_part, res_rect, max_alpha * (1.f - progress));
    if (mTexInd.elem_to == -1) return;

    const auto& tex_to = pTexMgr->GetLockerTexture(mTexInd.elem_to);
    const auto tracked_kp = dynamic_cast<FilmKeypointLayerInteractTransparentSwap*>(maEases.at(mTexInd.unused_padding).keypoint);

    switch (tracked_kp->swap) {
    default:
    case FilmKeypointLayerSwap::KeepInAspect: _FALLTHROUGH
        tex_to.renderRaw(res_part, res_rect, max_alpha * progress);
        break;
    case FilmKeypointLayerSwap::KeepNotDeformed:
        tex_to.render();
        break;
    case FilmKeypointLayerSwap::SetDefault:
        tex_to.renderRaw(nullptr, nullptr, max_alpha * progress);
        break;
    case FilmKeypointLayerSwap::NewTransform:
        tex_to.renderRaw(tracked_kp->swap_part_ptr.get(), tracked_kp->swap_rect_ptr.get(), max_alpha * progress);
        break;
    }
}

void FilmLayerTexture::finalizeSwap(LockerSimple<FilmLayerBase::Tracker>::Iterator iter) {
    const auto keypoint = dynamic_cast<FilmKeypointLayerInteractTransparentSwap*>(iter->keypoint);
    const auto swapmode = keypoint->swap;

    mRect.shift_elem();
    mPart.shift_elem();

    switch (swapmode) {
    case FilmKeypointLayerSwap::KeepNotDeformed:
    {
        const auto& tex = pTexMgr->GetLockerTexture(mTexInd.elem_to);
        mRect.elem_to = tex.getRectRes();
        mPart.elem_to = tex.getRectPart();
    }   break;
    case FilmKeypointLayerSwap::SetDefault:
        mRect.set_default();
        mPart.set_default();
        break;
    case FilmKeypointLayerSwap::NewTransform:
        if (keypoint->swap_rect_ptr)
            mRect.set_default();
        else mRect.elem_to = *keypoint->swap_rect_ptr;

        if (keypoint->swap_part_ptr)
             mPart.set_default();
        else mPart.elem_to = *keypoint->swap_part_ptr;
        break;
    }
}
