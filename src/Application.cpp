#include "Application.h"
#include "examples/ExampleFilmKeypoint2LayerSync.h"

#ifdef DEBUG
#include <stdio.h>
#endif

void Application::OnInit() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logCritical("SDL failed to initialize: %s.", SDL_GetError());
        exit(1);
    }

    if (!TTF_Init()) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logCritical("SDL_ttf failed to initialize: %s.", SDL_GetError());
        exit(1);
    }

    OpenWindow();

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
        printf("\r%d%%", (int)((float)mLoader.GetProgress() / (float)mLoader.Size() * 100.f));
    }
    printf("\r100%%\n");

    if (mLoader.IsFailed()) {
        Logger log(DEFAULT_LOG_PATH);
        log.logError("Resource wasn't loaded properly: %s.", mLoader.GetResourcePath(mLoader.GetFailed()));
        exit(1);
    }
    assert(mLoader.GetTranscription(0) == 0);
    assert(mLoader.GetTranscription(1) == 1);

    mScene.create(ScaleOption({ DEFAULT_SCR_RES_X, DEFAULT_SCR_RES_Y }), &mLoader);
    mScene.setClock(&mClock);

    film::KeypointLayerAddAnimation a;
    a.loaderind = mLoader.GetTranscription(0);
    mScene.addKeypoint(a);
    a.loaderind = mLoader.GetTranscription(1);
    mScene.addKeypoint(a);

    film::KeypointLayerInteractPos p;
    p.layerindx = 1;
    p.rect.x = 200;
    p.rect.y = 200;
    mScene.addKeypoint(p);

    film::KeypointLayerInteractAlpha ana;
    ana.layerindx = 0;
    ana.alpha = 128;
    mScene.addKeypoint(ana);

    film::KeypointLayerInteractAnimationLoop anl;
    anl.layerindx = 0;
    mScene.addKeypoint(anl);
    anl.layerindx = 1;
    mScene.addKeypoint(anl);

    film::KeypointLayerInteractAnimationStart ans;
    ans.layerindx = 0;
    mScene.addKeypoint(ans);
    ans.layerindx = 1;
    mScene.addKeypoint(ans);

    film::KeypointLayerEnable e;
    e.layerindx = 0;
    mScene.addKeypoint(e);
    e.layerindx = 1;
    mScene.addKeypoint(e);


    film::Keypoint ts;
    ts.action = ts.InInputAfterAwait;
    ts.frame_delay = 100;
    mScene.addKeypoint(ts);

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
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
    if (!mMainWindow.create("Wanichusilk", DEFAULT_SCR_RES_X, DEFAULT_SCR_RES_Y)) {
        Logger log(DEFAULT_LOG_PATH);
        log.logCritical("Couldn't create main window");
        exit(1);
    }
    SDL_SetRenderVSync(mMainWindow.getWindowRenderer(), false);
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
    mScene.update();
}

void Application::OnRender() {
    SDL_SetRenderDrawColor(mMainWindow.getWindowRenderer(), 100, 100 + 100, 200, 255);
    mScene.render();
}
