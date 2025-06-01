#include "Application.h"

void Application::OnInit() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // the loading of SDL was failed: log
        exit(1);
    }

    OpenWindow();

    mResMgr.SetRenderer(mMainWindow.getWindowRenderer());


    if (mResMgr.RequestTextureLoad("./Stefan chill emoji.png") == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load texture: %s\n", SDL_GetError());
    }

    mResMgr.GetLockerTexture(0).setOffset(256, 100);

    mTex.create("./Stefan chill emoji.png", mMainWindow.getWindowRenderer());
}

void Application::OnLoop() {
    PullEvents();

    SDL_RenderClear(mMainWindow.getWindowRenderer());

    OnRender();

    SDL_RenderPresent(mMainWindow.getWindowRenderer());
    SDL_Delay(10);
}

void Application::OpenWindow() {
    if (!mMainWindow.create("Wanichusilk", DEFAULT_SCR_RES_X, DEFAULT_SCR_RES_Y)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s\n", SDL_GetError());
    }
}

void Application::PullEvents() {
    while (SDL_PollEvent(&mEvent)) {
        switch (mEvent.type) {
        case SDL_EVENT_QUIT:
            mNeedQuit = true;
            break;
        default:
            break;
        }
    }
}

void Application::OnRender() {
    SDL_SetRenderDrawColor(mMainWindow.getWindowRenderer(), 200, 100, 200, 255);

    mResMgr.GetLockerTexture(0).render();

    mTex.render();
}
