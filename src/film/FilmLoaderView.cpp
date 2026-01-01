#include "FilmLoaderView.h"

bool film::LoaderView::registerLayerKeypoint(KeypointLoaderView* keypoint) {
    pKeypoint = keypoint;
    return false;
}

void film::LoaderView::render() {
    SDL_Color color;
    switch (pKeypoint->type()) {
    case KeypointLoaderView::Fill: {
        color = dynamic_cast<KeypointLoaderViewFill*>(pKeypoint)->color;
        SDL_SetRenderDrawColor(pRenderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(pRenderer);
    }   break;
    default:
        break;
    }

    SDL_FRect rect = { 0.f };
    float progress = (float)pLoader->GetProgress() / (float)pLoader->Size();

    switch (pKeypoint->progress_layout) {
    case KeypointLoaderView::BottomBar: {
        const auto frame = pScale->getFrameSize();
        rect.h = ((float)frame.height / 10.f);
        rect.y = frame.height - rect.h;
        rect.w = frame.width * progress;

        SDL_SetRenderDrawColor(renderer, 255 - color.r, 255 - color.g, 255 - color.b, color.a);
        SDL_RenderRect(pRenderer, rect);
    }    break;
    default:
        break;
    }
}
