#pragma once
#include <vector>
#include <list>
#include <memory>
#include <deque>

#include "FilmKeypointLayer.h"
#include "FilmLayerTexture.h"
#include "FilmLayerAnimation.h"
#include "FilmLayerText.h"
#include "../LockerSimple.h"
#include "../Clock.h"
#include "../EaseTracker.h"
#include "../rect_math.h"
#include "../PolyPointerList.h"
#include "../Loader.h"
#include "../Logger.h"

// it handles layer stuff, transition between positions, using ease functions, etc...

namespace film {
    class Layerist;
}

class film::Layerist : public ClockHolder, public LoaderHolder {
public:
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
};

