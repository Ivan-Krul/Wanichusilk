#pragma once

#include "FilmLayerBase.h"
#include "FilmKeypointLayer.h"
#include "../rect_math.h"

namespace film {
    class LayerRect;
}

class film::LayerRect : public film::LayerBase {
public:
    virtual void update() = 0;
    virtual void render() const = 0;
    virtual bool isWaiting() const noexcept { return maEases.isEmpty(); }
    virtual TimerStep getLongestWaiting() const noexcept { return mRect.ease_tracker.getLimiter(); }
    virtual void clear() { maEases.clear(); mRect.clear(); }

    virtual ~LayerRect() = default;
protected:
    template<typename K>
    inline typename std::enable_if<std::is_same<K, KeypointLayerInteractPos>::value>::type pushPosSetter(const KeypointLayer* keypoint);
    template<typename K>
    inline typename std::enable_if<std::is_same<K, KeypointLayerInteractRectPos>::value>::type pushPosSetter(const KeypointLayer* keypoint);
    template<typename K>
    inline void pushOtherPosSetter(const KeypointLayer* keypoint, TransitParam<SDL_FRect>& param);

    template<typename K>
    inline typename std::enable_if<std::is_same<K, KeypointLayerInteractPos>::value>::type pushPosTracker(const LockerIndex ease_indx);
    template<typename K>
    inline typename std::enable_if<std::is_same<K, KeypointLayerInteractRectPos>::value>::type pushPosTracker(const LockerIndex ease_indx);
    template<typename K>
    inline void pushOtherPosTracker(const LockerIndex ease_indx, TransitParam<SDL_FRect>& param);

    inline const SDL_FRect* computeRectRender(const TransitParam<SDL_FRect>& param, SDL_FRect* buffer) const noexcept;

    TransitParam<SDL_FRect> mRect;
};

inline const SDL_FRect* film::LayerRect::computeRectRender(const TransitParam<SDL_FRect>& param, SDL_FRect* buffer) const noexcept {
    if (param.is_default()) return nullptr;
    else {
        if (param.is_progress()) {
            *buffer = lerp_rect(param.elem_from, param.elem_to, param.ease_tracker);
            return buffer;
        }
        else return &(param.elem_to);
    }
}

template<typename K>
inline typename std::enable_if<std::is_same<K, film::KeypointLayerInteractPos>::value>::type film::LayerRect::pushPosSetter(const KeypointLayer* keypoint) {
    const auto kp = dynamic_cast<const K*>(keypoint);

    mRect.shift_elem();
    mRect.elem_to.x = kp->rect.x;
    mRect.elem_to.y = kp->rect.y;
    mRect.reset_tracker();
}
template<typename K>
inline typename std::enable_if<std::is_same<K, film::KeypointLayerInteractRectPos>::value>::type film::LayerRect::pushPosSetter(const KeypointLayer* keypoint) {
    const auto kp = dynamic_cast<const K*>(keypoint);

    mRect.shift_elem();
    mRect.elem_to = kp->rect;
    mRect.reset_tracker();
}
template<typename K>
inline void film::LayerRect::pushOtherPosSetter(const KeypointLayer* keypoint, TransitParam<SDL_FRect>& param) {
    const auto kp = dynamic_cast<const K*>(keypoint);

    param.shift_elem();
    param.elem_to = kp->rect;
    param.reset_tracker();
}

template<typename K>
inline typename std::enable_if<std::is_same<K, film::KeypointLayerInteractPos>::value>::type film::LayerRect::pushPosTracker(const LockerIndex ease_indx) {
    auto& tracker = maEases[ease_indx];
    const auto kp = dynamic_cast<const K*>(tracker.keypoint);

    pushTransitTracker(tracker, mRect);
    mRect.elem_to.x = kp->rect.x;
    mRect.elem_to.y = kp->rect.y;

}
template<typename K>
inline typename std::enable_if<std::is_same<K, film::KeypointLayerInteractRectPos>::value>::type film::LayerRect::pushPosTracker(const LockerIndex ease_indx) {
    auto& tracker = maEases[ease_indx];

    pushTransitTracker(tracker, mRect);
    mRect.elem_to = dynamic_cast<const K*>(tracker.keypoint)->rect;
}
template<typename K>
inline void film::LayerRect::pushOtherPosTracker(const LockerIndex ease_indx, TransitParam<SDL_FRect>& param) {
    auto& tracker = maEases[ease_indx];

    pushTransitTracker(tracker, mRect);
    param.elem_to = dynamic_cast<const K*>(tracker.keypoint)->rect;
}

