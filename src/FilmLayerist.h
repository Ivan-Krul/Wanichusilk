#pragma once
#include <vector>
#include <list>
#include <memory>
#include <deque>

#include "FilmKeypoint.h"
#include "LockerSimple.h"
#include "Clock.h"
#include "EaseTracker.h"
#include "FilmLayerTexture.h"
#include "rect_math.h"
#include "PolyPointerList.h"

// it handles layer stuff, transition between positions, using ease functions, etc...

        
class FilmLayerist : public ClockHolder {
public:
    inline void setTextureManager(TextureManager* texmgr) { pTexMgr = texmgr; }
    void registerLayerKeypoint(FilmKeypointLayer* keypoint);

    bool isWaiting() const;
    void update();
    void render();

    FilmTimer getLongestWaiting() const;
private:
    void registerLayerKeypointAdd(FilmKeypointLayerAdd* keypoint);
    void registerKeypointInteraction(LayerIndex li, FilmKeypointLayer* keypoint);
private:
    PolyPointerList<FilmLayerBase> maLayers;
    std::vector<PolyPointerList<FilmLayerBase>::Iterator> maActiveLayerIndexes;

    TextureManager* pTexMgr;
};

