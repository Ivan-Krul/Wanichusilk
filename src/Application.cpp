#include "Application.h"

void Application::OnInit() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // the loading of SDL was failed: log
        exit(1);
    }

    OpenWindow();

    mTexMgr.SetRenderer(mMainWindow.getWindowRenderer());

    std::vector<std::string> vec {
        "./Stefan chill emoji.png",
        "./Stefan is laying.png"
    };

    mScene.create(&mTexMgr, vec);

    FilmKeypointSwap swap;
    swap.need_input = true;
    swap.frame_delay = 60;
    swap.to = 0;
    mScene.addKeypoint(swap);
    swap.to = 1;
    mScene.addKeypoint(swap);
    swap.to = 0;
    mScene.addKeypoint(swap);
    swap.to = 1;
    mScene.addKeypoint(swap);
    swap.to = 0;
    mScene.addKeypoint(swap);
    swap.to = 1;
    mScene.addKeypoint(swap);
    swap.to = 0;
    mScene.addKeypoint(swap);
    swap.to = 1;
    mScene.addKeypoint(swap);
    swap.to = -1;
    mScene.addKeypoint(swap);

    mScene.start();
}

void Application::OnLoop() {
    PullEvents();
    OnUpdate();

    SDL_RenderClear(mMainWindow.getWindowRenderer());

    OnRender();

    SDL_RenderPresent(mMainWindow.getWindowRenderer());

    auto delay = mClock.FrameDelay(60);
    if (delay.count() > 0.0f) {
        SDL_Delay(static_cast<Uint32>(delay.count() * 1000.0f));
    }
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
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            mScene.next();
        default:
            break;
        }
    }
}

void Application::OnUpdate() {
    mClock.Update();
    mScene.update();
}

void Application::OnRender() {
    SDL_SetRenderDrawColor(mMainWindow.getWindowRenderer(), mScene.needNext() * 100+ 100, 100, 200, 255);

    mScene.render();
}
