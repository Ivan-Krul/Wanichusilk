#pragma once
#include <vector>
#include <list>
#include <memory>
#include <deque>

#include "FilmKeypointLayer.h"
#include "FilmLayerTexture.h"
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
    void registerLayerKeypoint(KeypointLayer* keypoint);

    bool isWaiting() const;
    void update();
    void render();

    TimerStep getLongestWaiting() const;
private:
    void registerLayerKeypointAdd(KeypointLayerAdd* keypoint);
    void registerKeypointInteraction(LayerIndex li, KeypointLayer* keypoint);
private:
    PolyPointerList<LayerBase> maLayers;
    std::vector<PolyPointerList<LayerBase>::Iterator> maActiveLayerIndexes;

    TextureManager* pTexMgr;
};

