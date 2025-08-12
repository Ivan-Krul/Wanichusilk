#pragma once
#include "FilmKeypoint.h"
#include "TextureManager.h"
#include "Clock.h"
#include "FrameScaling.h"
#include "EaseTracker.h"

class FilmBackground {
public:
    static constexpr float c_ease_no_progress = -0.f;
    static constexpr float c_ease_use_default = 2.f;

    inline void setTextureManager(TextureManager* texmgr) { pTexMgr = texmgr; }
    inline void setScaleOption(ScaleOption* scale) { pScale = scale; }
    void setClock(Clock* clock) { mEaseTimer.setClock(clock); }

    inline bool isWaiting() const { return !mEaseTimer.isProgress(); }
    void registerBackgroundKeypoint(FilmKeypointBackground* keypoint);
    void update();
    void render();
private:
    void transformTexture(ResourceIndex texind, FilmKeypointBackground::RenderMode rend_mode);
    void simplyPutTexture(ResourceIndex texind);
    void centerBlackBordersTexture(ResourceIndex texind);

    ResourceIndex mTexPrev;
    ResourceIndex mTex;
    FilmKeypointBackground::RenderMode mRendModePrev;
    FilmKeypointBackground::RenderMode mRendMode;
    EaseTracker<> mEaseTimer;

    FilmKeypointBackground* pKeypoint = nullptr;
    TextureManager* pTexMgr = nullptr;
    ScaleOption* pScale = nullptr;
};
