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
    inline TimerStep getLongestWaiting() const noexcept override;

    virtual ~LayerText() { clear(); }

private:
    inline void pushTextIndSetter(KeypointLayerInteractSwap* keypoint);

    bool onPushSetter(KeypointLayer* keypoint) override;
    bool onPushTracker(const LockerIndex ease_indx) override;

    inline bool areAllTransitParamDefault() const noexcept { return mColor.is_default() && mRect.is_default(); }

    TextManager* pTextMgr;
    Text* pText;
    
    TransitParam<SDL_Color> mColor;

    TextIndex mTextInd;

};
