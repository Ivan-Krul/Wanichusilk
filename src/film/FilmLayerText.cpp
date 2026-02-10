#include "FilmLayerText.h"

film::LayerText::LayerText(Clock* clock, TextManager* textmgr, LockerIndex textind) : pTextMgr(textmgr), mTextInd(textind) {
    setClock(clock);

    pText = textind ? pTextMgr->GetLockerResource(textind) : nullptr;
    mRect.elem_to = { 0.f };
    if (pText) {
        mRect.elem_to.w = pText->getWidth();
        mRect.elem_to.h = pText->getHeight();
    }
    mColor.elem_to = { 255 };

    mColor.ease_tracker.setClock(clock);

    mRect.reset_tracker();
}

void film::LayerText::update() {
    if (maEases.isEmpty()) return;
    mRect.ease_tracker.update();
    mColor.ease_tracker.update();

    auto it = maEases.begin();
    while (it != maEases.end()) {
        if (it->ease->isEnded())
            it = maEases.popFromLocker(it);
        else it++;
    }
}

void film::LayerText::render() const {
    assert(pText);
    SDL_FRect rect = { 0.f };
    SDL_Color color = { 255 };

    if (mRect.is_progress()) {
        rect = lerp_rect(mRect.elem_from, mRect.elem_to, mRect.ease_tracker);
    } else rect = mRect.elem_to;

    if (mColor.is_progress()) {
        color.r = lerp(mColor.ease_tracker, mColor.elem_from.r, mColor.elem_to.r);
        color.g = lerp(mColor.ease_tracker, mColor.elem_from.g, mColor.elem_to.g);
        color.b = lerp(mColor.ease_tracker, mColor.elem_from.b, mColor.elem_to.b);
        color.a = lerp(mColor.ease_tracker, mColor.elem_from.a, mColor.elem_to.a);
    } else color = mColor.elem_to;

    if (areAllTransitParamDefault())
        pText->render();
    else
        pText->renderRaw(rect, color);
}

void film::LayerText::clear() {
    maEases.clear();
    mRect.clear();
    mColor.clear();

    mTextInd = -1;
    pText = nullptr;
}

inline TimerStep film::LayerText::getLongestWaiting() const noexcept {
    return clockfunc::max(mColor.ease_tracker.getLimiter(), mRect.ease_tracker.getLimiter());
}

void film::LayerText::pushTextIndSetter(KeypointLayerInteractSwap* keypoint) {
    KeypointLayerInteractSwap::SwapMode swapmode = keypoint->swap;

    mRect.shift_elem();

    mTextInd = keypoint->indx;
    pText = mTextInd != -1 ? pTextMgr->GetLockerResource(mTextInd) : nullptr;

    switch (swapmode) {
    case KeypointLayerInteractSwap::KeepNotDeformed:
        if (pText == nullptr) break;
        mRect.elem_to = pText->getRect();
        break;
    case KeypointLayerInteractSwap::SetDefault:
        mRect.set_default();
        break;
    case KeypointLayerInteractSwap::NewTransform:
        if (!keypoint->swap_rect_ptr) {
            mRect.elem_to = *keypoint->swap_rect_ptr;
            mRect.reset_tracker();
        }
        else mRect.set_default();
        break;
    }
}

bool film::LayerText::onPushSetter(KeypointLayer* keypoint) {
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosSetter<KeypointLayerInteractPos>(keypoint);     break;
    case KeypointLayer::InteractRectPos: pushPosSetter<KeypointLayerInteractRectPos>(keypoint); break;
    case KeypointLayer::InteractColor:
    {
        const auto kp_color = dynamic_cast<KeypointLayerInteractColor*>(keypoint);
        mColor.shift_elem();
        mColor.ease_tracker.reset();
        mColor.elem_to = kp_color->color;
    }   break;
    case KeypointLayer::InteractAlpha:
    {
        const auto kp_color = dynamic_cast<KeypointLayerInteractAlpha*>(keypoint);
        mColor.elem_to.a = kp_color->alpha;
    }   break;
    case KeypointLayer::InteractDefaultPos:     mRect.set_default(); break;
    case KeypointLayer::InteractSwap:
        pushTextIndSetter(dynamic_cast<KeypointLayerInteractSwap*>(keypoint));
        break;
    case KeypointLayer::InteractTextWrap:
        if (pText) pText->setWrapPxLimit(dynamic_cast<KeypointLayerInteractTextWrap*>(keypoint)->wrap_limit_px);
        break;
    case KeypointLayer::InteractTextNew:
        if (pText) pText->setText(dynamic_cast<KeypointLayerInteractTextNew*>(keypoint)->new_text);
        break;
    case KeypointLayer::InteractTextAppend:
        if (pText) pText->appendText(dynamic_cast<KeypointLayerInteractTextAppend*>(keypoint)->text_at_end);
        break;
    case KeypointLayer::InteractTextInsert:
        if (pText) pText->insertText(dynamic_cast<KeypointLayerInteractTextInsert*>(keypoint)->offset,dynamic_cast<KeypointLayerInteractTextInsert*>(keypoint)->text_at_insert);
        break;
    case KeypointLayer::InteractTextSwapFont:
        if (pText) pText->setFontMgrIndex(dynamic_cast<KeypointLayerInteractTextSwapFont*>(keypoint)->fontindx);
        break;
    case KeypointLayer::InteractDefault:
        mRect.set_default();
        mColor.set_default();
        break;
    default:
        return true;
    }

    return false;
}

bool film::LayerText::onPushTracker(const LockerIndex ease_indx) {
    auto& tracker = maEases[ease_indx];
    const auto keypoint = tracker.keypoint;
    switch (keypoint->type().specific_type) {
    case KeypointLayer::InteractPos:     pushPosTracker<KeypointLayerInteractPos>(ease_indx);     break;
    case KeypointLayer::InteractRectPos: pushPosTracker<KeypointLayerInteractRectPos>(ease_indx); break;
    case KeypointLayer::InteractAlpha:
        pushTransitTracker(tracker, mColor);
        mColor.elem_to.a = dynamic_cast<KeypointLayerInteractAlpha*>(keypoint)->alpha;
        break;
    case KeypointLayer::InteractColor:
        pushTransitTracker(tracker, mColor);
        mColor.elem_to = dynamic_cast<KeypointLayerInteractColor*>(keypoint)->color;
        break;
    default:
        return true;
    }

    return false;
}
