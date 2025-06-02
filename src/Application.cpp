#include "Application.h"

void Application::OnInit() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // the loading of SDL was failed: log
        exit(1);
    }

    OpenWindow();

    mTexMgr.SetRenderer(mMainWindow.getWindowRenderer());

    if (mTexMgr.RequestTextureLoad("./Stefan chill emoji.png") == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load texture: %s\n", SDL_GetError());
    }
    if (mTexMgr.RequestTextureLoad("./Stefan is laying.png") == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load texture: %s\n", SDL_GetError());
    }
    if (mTexMgr.RequestTextureLoad("./Stefan is laying.png") == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load texture: %s\n", SDL_GetError());
    }
    mTexMgr.RequestTextureClean(1);
    if (mTexMgr.RequestTextureLoad("./Stefan chill emoji.png") == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load texture: %s\n", SDL_GetError());
    }

    mTexMgr.GetLockerTexture(0).setOffset(0, 0);
    mTexMgr.GetLockerTexture(1).setOffset(DEFAULT_SCR_RES_X - 256, DEFAULT_SCR_RES_Y - 256);
    mTexMgr.GetLockerTexture(2).setOffset(0,0);
    mTexMgr.GetLockerTexture(2).setResolution(DEFAULT_SCR_RES_X, DEFAULT_SCR_RES_Y);
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

    mTexMgr.GetLockerTexture(2).render();
    mTexMgr.GetLockerTexture(0).render();
    mTexMgr.GetLockerTexture(1).render();
}
