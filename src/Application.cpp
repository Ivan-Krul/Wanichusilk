#include "Application.h"
#include "examples/ExampleFilmKeypoint2LayerSync.h"

#ifdef DEBUG
#include <stdio.h>
#endif

void Application::OnInit() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logCritical("SDL failed to initialize: %s.", SDL_GetError());
        mNeedQuit = true;
        mIsCritical = true;
        return;
    }

    if (!TTF_Init()) {
        Logger log(DEFAULT_LOG_SDL_PATH);
        log.logCritical("SDL_ttf failed to initialize: %s.", SDL_GetError());
        mNeedQuit = true;
        mIsCritical = true;
        return;
    }

    OpenWindow();

    //pTEngine = TTF_CreateRendererTextEngine(mMainWindow.getWindowRenderer());
    //if (!pTEngine) {
    //    Logger log(DEFAULT_LOG_SDL_PATH);
    //    log.logCritical("SDL_ttf failed to create text engine: %s.", SDL_GetError());
    //    mNeedQuit = true;
    //    mIsCritical = true;
    //}

    mTexMgr.SetRenderer(mMainWindow.getWindowRenderer());
    mAnimMgr.SetRenderer(mMainWindow.getWindowRenderer());
    mTextMgr.SetRenderer(mMainWindow.getWindowRenderer());
    mTextMgr.SetFontManager(&mFontMgr);
    mAnimMgr.SetClock(&mClock);

    AnimationManager::LoadParamConvertor ap;
    ap.path = "./res/cat-runner-2049-cat-runner.gif";
    mLoader.PushResourcePathInQueue(&ap, &mAnimMgr);
    ap.path = "./res/lancer-spin-big.gif";
    mLoader.PushResourcePathInQueue(&ap, &mAnimMgr);
    FontManager::LoadParamConvertor fp;
    fp.size = 25.f;
    fp.path = "./res/TerminusTTF-Bold.ttf";
    mLoader.PushResourcePathInQueue(&fp, &mFontMgr);
    fp.path = "./res/Scheherazade-Regular.ttf";
    mLoader.PushResourcePathInQueue(&fp, &mFontMgr);
    fp.path = "./res/wingding.ttf";
    mLoader.PushResourcePathInQueue(&fp, &mFontMgr);
    fp.path = "./res/FSEX302.ttf";
    mLoader.PushResourcePathInQueue(&fp, &mFontMgr);
    TextManager::LoadParamConvertor tp;
    tp.text = u8"What would you think?\nWorüber denkst du?\nПро що ти думаєш?\nO czym myślisz?\n¿En qué piensas?\nNe düşünüyorsun?\nApa pendapat Anda?";
    tp.font_indx = 0;
    mLoader.PushResourcePathInQueue(&tp, &mTextMgr);
    tp.text = u8"يرحب بالمكان الهادئ بالإلهام";
    tp.font_indx = 1;
    mLoader.PushResourcePathInQueue(&tp, &mTextMgr);
    tp.text = u8"WHAT A QUIET PLACE HERE";
    tp.font_indx = 2;
    mLoader.PushResourcePathInQueue(&tp, &mTextMgr);
    tp.text = u8"A 也许，友谊是人生中最美好的东西";
    tp.font_indx = 3;
    mLoader.PushResourcePathInQueue(&tp, &mTextMgr);

    SCOPED_STOPWATCH("load");

    mLoader.Load();

    while (mLoader.IsProgress()) { // async loading
        printf("\r%d%%", (int)((float)mLoader.GetProgress() / (float)mLoader.Size() * 100.f));
    }
    printf("\r100%%\n");

    if (mLoader.IsFailed()) {
        Logger log(DEFAULT_LOG_PATH);
        log.logError("Resource wasn't loaded properly: %s.", mLoader.GetResourcePath(mLoader.GetFailed()));
        mNeedQuit = true;
        mIsCritical = true;
        return;
    }
    
    if (mLoader.Preprocess()) {
        Logger log(DEFAULT_LOG_PATH);
        log.logError("Resource wasn't preprocessed properly: %s.", mLoader.GetResourcePath(mLoader.GetFailed()));
        mNeedQuit = true;
        mIsCritical = true;
        return;
    }

    ScaleOption so;
    so.p_wind = mMainWindow.getWindow();

    assert(!mScene.create(so, &mLoader));
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

    //mAnimMgr.GetLockerResource(0)->start(1.f);
    mScene.start();

    mTextMgr.GetLockerResource(0)->setOffsetX(100);
    mTextMgr.GetLockerResource(0)->setOffsetY(50);

    mTextMgr.GetLockerResource(2)->setOffsetX(300);
    TTF_SetTextColor(mTextMgr.GetLockerResource(3)->getTextInstance(), 255, 255, 0, 255);
    mTextMgr.GetLockerResource(3)->setOffsetY(300);

    //mAnimMgr.GetLockerResource(0)->setAlpha(128);
    //mAnimMgr.GetLockerResource(0)->setLooping(true);
    //mAnimMgr.GetLockerResource(0)->setFreeze(false);
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

int Application::OnExit() {
    return mIsCritical;
}

void Application::OpenWindow() {
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
    if (!mMainWindow.create("Wanichusilk", DEFAULT_SCR_RES_X, DEFAULT_SCR_RES_Y, SDL_WINDOW_RESIZABLE)) {
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
            break;
        case SDL_EVENT_KEY_DOWN:
            mNeedQuit = mEvent.key.key == SDLK_ESCAPE || mNeedQuit;
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            SDL_RenderViewportSet(mMainWindow.getWindowRenderer());
            break;
        default:

            break;
        }
    }
}

void Application::OnUpdate() {
    mScene.update();
}

void Application::OnRender() {
    SDL_SetRenderDrawColor(mMainWindow.getWindowRenderer(), 100, 100, 200, 255);
    //mAnimMgr.GetLockerResource(0)->render();
    //mAnimMgr.GetLockerResource(1)->render();
    mScene.render();
    //mAnimMgr.GetLockerResource(0)->render();
    mTextMgr.GetLockerResource(0)->render();
    mTextMgr.GetLockerResource(1)->render();
    mTextMgr.GetLockerResource(2)->render();
    mTextMgr.GetLockerResource(3)->render();
}
