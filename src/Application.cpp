#include "Application.h"

void Application::OnInit() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // the loading of SDL was failed: log
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
    OpenWindow();
    SDL_SetRenderVSync(mMainWindow.getWindowRenderer(), false);

    mTexMgr.SetRenderer(mMainWindow.getWindowRenderer());

    std::vector<std::string> vec {
        "./Stefan chill emoji.png",
        "./Stefan is laying.png"
    };

    mScene.create(&mTexMgr, vec);

    FilmKeypointSwap swap;
    swap.need_input = true;
    swap.frame_delay = 50;
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
    mClock.StartMeasure();

    PullEvents();
    OnUpdate();

    SDL_RenderClear(mMainWindow.getWindowRenderer());
    OnRender();
    SDL_RenderPresent(mMainWindow.getWindowRenderer());

    mClock.FinishMeasure(50);

    //float delta = mClock.DeltaTime().count();
    //SDL_Log("dt %zu: %.2fms (%.1f FPS)", mCount++, delta * 1000.0f, 1.0f / delta);
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
    mScene.update();
}

void Application::OnRender() {
    SDL_SetRenderDrawColor(mMainWindow.getWindowRenderer(), mScene.needNext() * 100+ 100, 100, 200, 255);

    mScene.render();
}
