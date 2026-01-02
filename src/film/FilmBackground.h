#pragma once
#include "FilmKeypointBackground.h"
#include "../FrameScaling.h"
#include "../EaseTracker.h"
#include "../TextureManager.h"
#include "../Loader.h"

namespace film {
    class Background;
}

class film::Background {
public:
    inline void setLoader(Loader* loader) noexcept { pLoader = loader; pRenderer = dynamic_cast<IRendererGiver*>(pLoader->GetRequiredInterface(ResourceManagerAttribute::RendererGiver))->GetRenderer(); }
    inline void setScaleOption(ScaleOption* scale) { pScale = scale; }
    void setClock(Clock* clock) { mEaseTimer.setClock(clock); }

    inline bool isWaiting() const { return !mEaseTimer.isProgress(); }
    void registerBackgroundKeypoint(KeypointBackground* keypoint);
    void update();
    void render();

    inline TimerStep getLongestWaiting() const noexcept { return mEaseTimer.getLimiter(); }
private:
    void transformTexture(TextureIndex texind, KeypointBgTexSwap::RenderMode rend_mode);
    void simplyPutTexture(TextureIndex texind);
    void centerBlackBordersTexture(TextureIndex texind);

    //remove all texture related soon
    TextureIndex mTexPrev;
    TextureIndex mTex;
    EaseTracker<> mEaseTimer;

    SDL_Color mFill;

    KeypointBgTexSwap::RenderMode mRendMode;
    KeypointBgTexSwap::RenderMode mPrevRendMode;

    KeypointBackground* pKeypoint = nullptr;
    SDL_Renderer* pRenderer = nullptr;
    ScaleOption* pScale = nullptr;
    Loader* pLoader = nullptr;

    TextureManager* pTexMgr = nullptr;
};
