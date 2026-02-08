#pragma once
#include "FilmLayerRect.h"
#include "FilmKeypointLayer.h"
#include "../TextManager.h"

namespace film {
    class LayerText;
}

class film::LayerText : public LayerRect {
public:
    LayerText(Clock* clock, TextManager* textmgr, TextIndex textind);
    void update() override;
    void render() const override;
    void clear() override;
    inline bool isWaiting() const noexcept override;
    inline TimerStep getLongestWaiting() const noexcept override;

    virtual ~LayerText() { clear(); }

private:
    inline void pushTexIndSetter(KeypointLayerInteractSwap* keypoint);

    bool onPushSetter(KeypointLayer* keypoint) override;
    bool onPushTracker(const LockerIndex ease_indx) override;

    inline bool areAllTransitParamDefault() const noexcept { return mPart.is_default() && mRect.is_default() && mAlpha.is_default(); }

    TextManager* pTextMgr;
    
    TransitParam<SDL_Color> mColor;

    TextIndex mTextInd;

    Text* pText;
};
