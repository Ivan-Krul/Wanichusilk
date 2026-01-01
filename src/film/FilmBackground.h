#pragma once
#include "FilmKeypointBackground.h"
#include "../FrameScaling.h"
#include "../EaseTracker.h"
#include "../TextureManager.h"
#include "../Loader.h"

namespace film {
    class Background;
}

class film::Background : public LoaderHolder {
public:
    inline void setTextureManager(TextureManager* texmgr) { pTexMgr = texmgr; }
    inline void setScaleOption(ScaleOption* scale) { pScale = scale; }
    void setClock(Clock* clock) { mEaseTimer.setClock(clock); }

    inline bool isWaiting() const { return !mEaseTimer.isProgress(); }
    void registerBackgroundKeypoint(KeypointBackground* keypoint);
    void update();
    void render();

    inline TimerStep getLongestWaiting() const noexcept { return mEaseTimer.getLimiter(); }
private:
    void transformTexture(TextureIndex texind, KeypointBackground::RenderMode rend_mode);
    void simplyPutTexture(TextureIndex texind);
    void centerBlackBordersTexture(TextureIndex texind);

    TextureIndex mTexPrev;
    TextureIndex mTex;
    KeypointBackground::RenderMode mRendModePrev;
    KeypointBackground::RenderMode mRendMode;
    EaseTracker<> mEaseTimer;

    KeypointBackground* pKeypoint = nullptr;
    TextureManager* pTexMgr = nullptr;
    ScaleOption* pScale = nullptr;
};
