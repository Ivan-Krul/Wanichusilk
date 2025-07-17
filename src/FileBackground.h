#pragma once
#include "FilmKeypoint.h"
#include "TextureManager.h"
#include "Clock.h"

class FilmBackground {
public:
    void registerBackgroundKeypoint(FilmKeypoint* keypoint);
    void update();
    void render();
private:
    ResourceIndex mTex;

    Clock::SteadyClock::time_point mPrev = Clock::SteadyClock::now();

    TextureManager* pTexMgr;
    Clock* pClock;
};
