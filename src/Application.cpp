#include "Application.h"
#include "examples/ExampleFilmKeypoint2LayerSync.h"

#ifdef DEBUG
#include <stdio.h>
#endif

void Application::OnInit() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
#ifdef DEBUG
        printf("SDL failed to load: %s\n", SDL_GetError());
#endif
        // the loading of SDL was failed: log
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
    OpenWindow();
    SDL_SetRenderVSync(mMainWindow.getWindowRenderer(), false);


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

    mTexMgr.SetRenderer(mMainWindow.getWindowRenderer());
    mAnimMgr.SetRenderer(mMainWindow.getWindowRenderer());
    mAnimMgr.SetClock(&mClock);

    mLoader.PushResourcePathInQueue("./res/cat-runner-2049-cat-runner.gif", &mAnimMgr);
    mLoader.PushResourcePathInQueue("./res/lancer-spin-big.gif", &mAnimMgr);

    SCOPED_STOPWATCH("anim load");

    mLoader.Load();

    while (mLoader.IsProgress()) { // async loading
        SDL_Log("Progress: %d%%\r", (int)((float)mLoader.GetProgress() / (float)mLoader.Size() * 100.f));
    }

    SDL_Log("\n");

    if (mLoader.IsFailed()) {
        SDL_Log("Error while loading: %zu\n", mLoader.GetFailed());
        assert(false);
    }

    mScene.create(ScaleOption({ DEFAULT_SCR_RES_X, DEFAULT_SCR_RES_Y }), &mLoader);


    assert(mLoader.GetTranscription(0) == 0);
    assert(mLoader.GetTranscription(1) == 1);

    auto anim = mAnimMgr.GetLockerResource(mLoader.GetTranscription(0));
    anim->setClock(&mClock);
    anim->setLooping(true);
    anim->setAlpha(128);
    anim->start();
    anim->lockChange();
    auto res = anim->getRectRes();
    anim->setRectRes(lerp_rect(res, SDL_FRect{ 0.f }, 0.5f));
    SDL_Log("isBig: %d", anim->isBig() ? 1 : 0);

    anim = mAnimMgr.GetLockerResource(mLoader.GetTranscription(1));
    anim->setClock(&mClock);
    anim->setLooping(true);
    anim->start();
    anim->lockChange();
    res = anim->getRectRes();
    res.x = 200;
    res.y = 200;
    anim->setRectRes(res);
    SDL_Log("isBig: %d", anim->isBig() ? 1 : 0);
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
    mAnimMgr.GetLockerResource(0)->render();
    mAnimMgr.GetLockerResource(1)->render();

    //mScene.render();
}
