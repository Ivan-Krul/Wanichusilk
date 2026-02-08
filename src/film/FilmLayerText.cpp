#include "FilmLayerText.h"

film::LayerText::LayerText(Clock* clock, TextManager* textmgr, LockerIndex textind) {
    pTextMgr = textmgr;
    setClock(clock);

    mTextInd = textind;
    pText = textind ? pTextMgr->GetLockerResource(textind) : nullptr;
    if (pText) {
        mRect.elem_to.w = pText->getWidth();
        mRect.elem_to.h = pText->getHeight();
        mFontInd = pText->getFontIndex();
    }
    else mRect.elem_to = { 0.f };
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
}

void film::LayerText::clear() {
}

inline bool film::LayerText::isWaiting() const noexcept {
    return false;
}

inline TimerStep film::LayerText::getLongestWaiting() const noexcept {
    return TimerStep();
}

//,
//InteractTextNew,
//InteractTextAppend,
//InteractTextInsert,
//InteractTextSwapFont,

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
    case KeypointLayer::InteractDefaultPos:     mRect.set_default(); break;
    case KeypointLayer::InteractSwap:
        pushTexIndSetter(dynamic_cast<KeypointLayerInteractSwap*>(keypoint));
        break;
    case KeypointLayer::InteractTextWrap:

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
    return false;
}
