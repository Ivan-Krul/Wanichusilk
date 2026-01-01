#pragma once
#include <SDL3/SDL.h>

#include "FilmKeypointLoaderView.h"
#include "../Loader.h"
#include "../FrameScaling.h"

namespace film {
    class LoaderView;
}

class film::LoaderView : public LoaderHolder {
public:
    inline void setScaleOption(ScaleOption* scale) noexcept { pScale = scale; }
    inline void setRenderer(SDL_Renderer* renderer) noexcept { pRenderer = renderer; }
    bool registerLayerKeypoint(KeypointLoaderView* keypoint);

    inline bool isWaiting() const noexcept { return pLoader ? pLoader->IsLoaded() || pLoader->IsFailed() || pLoader->IsCleaned() : nullptr; }
    void update() {}
    void render();

private:
    KeypointLoaderView* pKeypoint;
    SDL_Renderer* pRenderer;
    ScaleOption* pScale = nullptr;
};
