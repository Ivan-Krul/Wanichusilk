#include "Application.h"

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

    OpenWindow(); // needs to add SDL_GetRenderSafeArea for future android support

    mImagMgr.SetRenderer(mMainWindow.getWindowRenderer());
    mAnimMgr.SetRenderer(mMainWindow.getWindowRenderer());
    mTextMgr.SetRenderer(mMainWindow.getWindowRenderer());
    mDrawer.setRenderer(mMainWindow.getWindowRenderer());
    mTextMgr.SetFontManager(&mFontMgr);
    mAnimMgr.SetClock(&mClock);

    // resource loading here
    ImageManager::LoadParamConvertor imagload;
    imagload.path = "./res/character.png";
    mLoader.PushResourcePathInQueue(&imagload, &mImagMgr);
    //AnimationManager::LoadParamConvertor animload;
    //animload.path = "./res/cat-runner-2049-cat-runner.gif";
    //mLoader.PushResourcePathInQueue(&animload, &mAnimMgr);
    //FontManager::LoadParamConvertor fontload;
    //fontload.path = "./res/unifont-17.0.03.otf";
    //fontload.size = 32.f;
    //mLoader.PushResourcePathInQueue(&fontload, &mFontMgr);
    //TextManager::LoadParamConvertor textload;
    //textload.font_indx = 0;
    //textload.text = u8"Bunnies!!!";
    //mLoader.PushResourcePathInQueue(&textload, &mTextMgr);
    //animload.path = "./res/received_1095637438226501.gif";
    //mLoader.PushResourcePathInQueue(&animload, &mAnimMgr);
 
    SCOPED_STOPWATCH("load");
 
    auto surf = SDL_GetWindowSurface(mMainWindow.getWindow());
    mDrawer.addColorGroup(SDL_Color{ 255,255,255,255 });
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.h = 32;

    mLoader.Load();

    while (mLoader.IsProgress()) { // async loading
        SDL_GetWindowSize(mMainWindow.getWindow(), &rect.w, nullptr);
        rect.w *= ((float)mLoader.GetProgress() / (float)mLoader.Size());
        PullEvents();
        SDL_ClearSurface(surf, 0.f, 0.f, 0.f, 1.f);
        SDL_FillSurfaceRect(surf, &rect, SDL_MapSurfaceRGB(surf, 255, 255, 255));
        SDL_UpdateWindowSurface(mMainWindow.getWindow());
        SDL_Delay(1);
    }
    mDrawer.removeColorGroup(0);
    SDL_DestroyWindowSurface(mMainWindow.getWindow());

    if (mLoader.IsFailed()) {
        Logger log(DEFAULT_LOG_PATH);
        log.logCritical("Resource wasn't loaded properly: %s.", mLoader.GetResourcePath(mLoader.GetFailed()));
        mNeedQuit = true;
        mIsCritical = true;
        return;
    }
 
    if (mLoader.Preprocess()) {
        Logger log(DEFAULT_LOG_PATH);
        log.logCritical("Resource wasn't preprocessed properly: %s.", mLoader.GetResourcePath(mLoader.GetFailed()));
        mNeedQuit = true;
        mIsCritical = true;
        return;
    }
 
	//SDL_SetTextureScaleMode(mTexMgr.GetLockerResource(0)->getTexture(), SDL_SCALEMODE_NEAREST);
 
    ScaleOption so;
    so.p_wind = mMainWindow.getWindow();
 
    assert(!mScene.create(so, &mLoader));
    mScene.setClock(&mClock);
 
    // all keypoints here
    //film::KeypointLayerAddGroup addg;
    //mScene.addKeypoint(addg); // l: 0
    //mScene.addKeypoint(addg); // l: 1

    film::KeypointLayerAddImage addi;
    addi.loaderind = 0;
    mScene.addKeypoint(addi); // l: 0

    film::KeypointLayerInteractRectPos irep;
    irep.layerindx = 0;
    irep.rect.x = 16;
    irep.rect.y = 16;
    irep.rect.h = 128;
    irep.rect.w = 128;
    mScene.addKeypoint(irep);

    film::KeypointLayerInteractSnapPos isap;
    isap.layerindx = 0;
    isap.rect.y = 0;
    isap.rect.x = 16;
    isap.rect.w = 16;
    isap.rect.h = 16;
    mScene.addKeypoint(isap);

    film::KeypointLayerEnable enab;
    enab.layerindx = 0;
    mScene.addKeypoint(enab);

    isap.set_delay_time(std::chrono::seconds(1));
    isap.action = isap.Exact;
    mScene.addKeypoint(isap);

    isap.rect.x = 0;
    mScene.addKeypoint(isap);

    isap.rect.x = 16;
    mScene.addKeypoint(isap);

    isap.rect.x = 0;
    mScene.addKeypoint(isap);

    isap.rect.x = 16;
    mScene.addKeypoint(isap);

    isap.rect.x = 0;
    mScene.addKeypoint(isap);

    film::Keypoint ts;
    ts.action = ts.InInputAfterAwait;
    mScene.addKeypoint(ts);

    mScene.start();
}

void Application::OnLoop() {
    mClock.StartMeasure();

    PullEvents();
    OnUpdate();

    SDL_SetRenderDrawColor(mMainWindow.getWindowRenderer(), 100, 100, 200, 255);
    SDL_RenderClear(mMainWindow.getWindowRenderer());
    OnRender();
    SDL_RenderPresent(mMainWindow.getWindowRenderer());

    mClock.FinishMeasure();
    if (!mClock.IsCounting()) {
        SDL_SetWindowTitle(mMainWindow.getWindow(), (std::string("Wanichusilk: ") + std::to_string(mClock.GetFPS())).c_str());
    }

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
            if (mLoader.IsLoaded()) mScene.next();
            if (mLoader.IsLoaded()) mNeedQuit = mScene.isEnded();
            break;
        case SDL_EVENT_KEY_DOWN:
            mNeedQuit = mEvent.key.key == SDLK_ESCAPE || mNeedQuit;
            break;
        case SDL_EVENT_KEY_UP:
            if (mEvent.key.key == SDLK_F11) {
                SDL_SetWindowFullscreen(mMainWindow.getWindow(), mIsFullscreen);
                mIsFullscreen = !mIsFullscreen;
            }

            break;
        case SDL_EVENT_WINDOW_RESIZED:
            if(mLoader.IsLoaded()) SDL_RenderViewportSet(mMainWindow.getWindowRenderer());
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
    mScene.render();
    mDrawer.render();
}
