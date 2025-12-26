#pragma once
#include <vector>
#include <list>
#include <memory>
#include <deque>

#include "FilmKeypointLayer.h"
#include "FilmLayerTexture.h"
#include "FilmLayerAnimation.h"
#include "../LockerSimple.h"
#include "../Clock.h"
#include "../EaseTracker.h"
#include "../rect_math.h"
#include "../PolyPointerList.h"

// it handles layer stuff, transition between positions, using ease functions, etc...

namespace film {
    class Layerist;
}

class film::Layerist : public ClockHolder {
public:
    inline void setTextureManager(TextureManager* texmgr) { pTexMgr = texmgr; }
    inline void setAnimationManager(AnimationManager* animmgr) { pAnimMgr = animmgr; }
    bool registerLayerKeypoint(KeypointLayer* keypoint);

    bool isWaiting() const;
    void update();
    void render();

    TimerStep getLongestWaiting() const;

private:
    inline bool registerLayerKeypointAdd(KeypointLayerAdd* keypoint);
    inline bool registerKeypointInteraction(LayerIndex li, KeypointLayer* keypoint);

private:
    PolyPointerList<LayerBase> maLayers;
    std::vector<PolyPointerList<LayerBase>::Iterator> maActiveLayerIndexes;

    TextureManager*   pTexMgr  = nullptr;
    AnimationManager* pAnimMgr = nullptr;
};

