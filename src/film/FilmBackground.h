#pragma once
#include "FilmKeypoint.h"
#include "../FrameScaling.h"
#include "../EaseTracker.h"
#include "../TextureManager.h"

class FilmBackground {
public:
    inline void setTextureManager(TextureManager* texmgr) { pTexMgr = texmgr; }
    inline void setScaleOption(ScaleOption* scale) { pScale = scale; }
    void setClock(Clock* clock) { mEaseTimer.setClock(clock); }

    inline bool isWaiting() const { return !mEaseTimer.isProgress(); }
    void registerBackgroundKeypoint(FilmKeypointBackground* keypoint);
    void update();
    void render();

    inline TimerStep getLongestWaiting() const noexcept { return mEaseTimer.getLimiter(); }
private:
    void transformTexture(TextureIndex texind, FilmKeypointBackground::RenderMode rend_mode);
    void simplyPutTexture(TextureIndex texind);
    void centerBlackBordersTexture(TextureIndex texind);

    TextureIndex mTexPrev;
    TextureIndex mTex;
    FilmKeypointBackground::RenderMode mRendModePrev;
    FilmKeypointBackground::RenderMode mRendMode;
    EaseTracker<> mEaseTimer;

    FilmKeypointBackground* pKeypoint = nullptr;
    TextureManager* pTexMgr = nullptr;
    ScaleOption* pScale = nullptr;
};
