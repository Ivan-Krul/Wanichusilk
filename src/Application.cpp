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
        "./res/Stefan chill emoji.png",
        "./res/Stefan is laying.png",
        "./res/Stefan wah.png"
    };

    assert(mScene.create(&mTexMgr, ScaleOption({ DEFAULT_SCR_RES_X, DEFAULT_SCR_RES_Y }), vec));
    mScene.setClock(&mClock);

    FilmKeypointBgTransparentSwap swap;
    swap.need_input = true;
    swap.frame_delay = 50;
    swap.from = -1;
    swap.to = 0;
    mScene.addKeypoint(swap);
    swap.need_input = false;
    swap.from = 0;
    swap.to = 1;
    mScene.addKeypoint(swap);
    swap.from = 1;
    swap.to = 0;
    mScene.addKeypoint(swap);
    {
        FilmKeypointLayerAdd la;
        la.texind = 2;
        la.need_await = true;
        mScene.addKeypoint(la);
    }
    {
        FilmKeypointLayerInteractAlpha lia;
        lia.layerindx = 0;
        lia.alpha = 0;
        mScene.addKeypoint(lia);
    }
    {
        FilmKeypointLayerEnable le;
        le.layerindx = 0;
        mScene.addKeypoint(le);
    }
    {
        FilmKeypointLayerInteractAlpha lia;
        lia.layerindx = 0;
        lia.alpha = 255;
        //lia.ease_func = ease_cubic_in_out;
        //lia.need_time_delay = true;
        lia.frame_delay = 180;
        mScene.addKeypoint(lia);
    }
    swap.from = 0;
    swap.to = 1;
    mScene.addKeypoint(swap);
    swap.from = -1;
    swap.to = 1;
    mScene.addKeypoint(swap);
    swap.need_input = true;
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
