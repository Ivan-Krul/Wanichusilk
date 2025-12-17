#include "Application.h"
#include "examples/ExampleFilmKeypoint2LayerSync.h"

void Application::OnInit() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // the loading of SDL was failed: log
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
    OpenWindow();
    SDL_SetRenderVSync(mMainWindow.getWindowRenderer(), false);

    //mTexMgr.SetRenderer(mMainWindow.getWindowRenderer());

    //std::vector<std::string> vec {
    //    "./res/Stefan chill emoji.png",
    //    "./res/Stefan is laying.png",
    //    "./res/Stefan wah.png",
    //    "./res/B.png"
    //};
    //
    //assert(mScene.create(&mTexMgr, ScaleOption({ DEFAULT_SCR_RES_X, DEFAULT_SCR_RES_Y }), vec));
    //mScene.setClock(&mClock);
    //
    //PushExampleFilmKeypoint2LayerSync(mScene);
    //
    //mScene.start();

    SCOPED_STOPWATCH("anim load");
    assert(mAnim.create("./res/received_1095637438226501.gif", mMainWindow.getWindowRenderer()));
    mAnim.setClock(&mClock);
    mAnim.setLooping(true);
    mAnim.setAlpha(128);
    mAnim.start();
    mAnim.lockChange();
    auto res = mAnim.getRectRes();
    mAnim.setRectRes(lerp_rect(res, SDL_FRect{ 0.f }, 0.5f));
}

void Application::OnLoop() {
    mClock.StartMeasure();

    PullEvents();
    OnUpdate();

    SDL_RenderClear(mMainWindow.getWindowRenderer());
    OnRender();
    SDL_RenderPresent(mMainWindow.getWindowRenderer());

    auto res = mAnim.getRectRes();
    res.x += .2f;
    res.y += .1f;
    mAnim.setRectRes(res);

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
    //if (mScene.isEnded()) {
    //    mNeedQuit = true;
    //}
    while (SDL_PollEvent(&mEvent)) {
        switch (mEvent.type) {
        case SDL_EVENT_QUIT:
            mNeedQuit = true;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            mNeedQuit = true;
        default:
            break;
        }
    }
}

void Application::OnUpdate() {
    //mScene.update();
}

void Application::OnRender() {
    SDL_SetRenderDrawColor(mMainWindow.getWindowRenderer(), 100, 100 + 100, 200, 255);
    mAnim.render();

    //mScene.render();
}
