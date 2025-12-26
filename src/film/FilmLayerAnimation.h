#pragma once
#include "FilmLayerRect.h"
#include "FilmKeypointLayer.h"
#include "../AnimationManager.h"

namespace film {
    class LayerAnimation;
}

class film::LayerAnimation : public LayerRect {
public:
    LayerAnimation(Clock* clock, AnimationManager* animmgr, AnimationIndex animind);
    void update() override;
    void render() const override;
    inline void clear() override;
    inline TimerStep getLongestWaiting() const noexcept override;

    virtual ~LayerAnimation() { clear(); }
private:
    inline void pushAnimIndSetter(KeypointLayer* keypoint);

    bool onPushSetter(KeypointLayer* keypoint) override;
    bool onPushTracker(const LockerIndex ease_indx) override;

    inline void renderSwap(const SDL_FRect* res_rect, uint8_t max_alpha, float time_mult) const;
    inline void finalizeSwap(LockerSimple<LayerBase::Tracker>::Iterator iter);

    inline bool areAllTransitParamDefault() const noexcept { return mRect.is_default() && mAlpha.is_default() && mAnimInd.is_default() && mTimeMult.is_default(); }

    AnimationManager* pAnimMgr;

    // no partition param btw
    TransitParam<uint8_t> mAlpha;
    TransitParam<AnimationIndex> mAnimInd;
    TransitParam<float> mTimeMult;

    Animation* pAnimation;
};
