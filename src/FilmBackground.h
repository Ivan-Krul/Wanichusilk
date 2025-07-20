#pragma once
#include "FilmKeypoint.h"
#include "TextureManager.h"
#include "Clock.h"

class FilmBackground {
public:
    enum RenderMode {
        blank,
        simple,
        centered_black_borders,
        //centered_fill,
        //scroll
    };
    inline void setTextureManager(TextureManager* texmgr) { pTexMgr = texmgr; }
    inline void setClock(Clock* clock) { pClock = clock; }

    void registerBackgroundKeypoint(FilmKeypoint* keypoint);
    void update();
    void render();
private:
    void centerBlackBordersTexture(ResourceIndex texind);

    ResourceIndex mTexPrev;
    ResourceIndex mTex;
    RenderMode mRendModePrev;
    RenderMode mRendMode;
    FilmTimer mTimer;

    FilmKeypoint* pKeypoint;
    TextureManager* pTexMgr;

    Clock::SteadyClock::time_point mPrev = Clock::SteadyClock::now();
    Clock* pClock;
};
