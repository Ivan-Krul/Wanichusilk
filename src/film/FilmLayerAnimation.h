#pragma once
#include "FilmLayerBase.h"
#include "FilmKeypointLayer.h"
#include "../rect_math.h"
#include "../AnimationManager.h"

namespace film {
    class LayerAnimation;
}

class film::LayerAnimation : public LayerBase {
public:
    LayerAnimation(Clock* clock, AnimationManager* animmgr, AnimationIndex animind);
    void update() override;
    void render() const override;
    inline void clear() override;
    inline TimerStep getLongestWaiting() const noexcept override;

    virtual ~LayerAnimation() { clear(); }
private:
    enum PosChangeEnum {
        Pos,
        RectPos,
        PartPos
    };
private:

    inline void pushPosSetter(const KeypointLayerInteractRect* keypoint, PosChangeEnum change);
    //inline void pushTexIndSetter(KeypointLayerInteractSwap* keypoint);

    bool onPushSetter(KeypointLayer* keypoint) override;

    //inline void pushPosTracker(const LockerIndex ease_indx, PosChangeEnum change);

    bool onPushTracker(const LockerIndex ease_indx) override;

    //void renderSwap(const SDL_FRect* res_rect, const SDL_FRect* res_part, uint8_t max_alpha) const;
    //void finalizeSwap(LockerSimple<LayerBase::Tracker>::Iterator iter);

    inline bool areAllTransitParamDefault() const noexcept { return mPart.is_default() && mRect.is_default() && mAlpha.is_default() && mAnimInd.is_default() && mTimeMult.is_default(); }

    AnimationManager* pAnimMgr;

    TransitParam<SDL_FRect> mPart;
    TransitParam<SDL_FRect> mRect;
    TransitParam<uint8_t> mAlpha;
    TransitParam<AnimationIndex> mAnimInd;
    TransitParam<float> mTimeMult;

    Animation* pAnimation;
};

