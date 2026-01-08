#pragma once
#include <SDL3/SDL.h>

#include "Window.h"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "FontManager.h"
#include "FilmScene.h"
#include "Clock.h"
#include "define.h"
#include "Stopwatch.h"

class Application {
public:
    void OnInit();
    void OnLoop();


    inline bool IsRunning() { return !mNeedQuit; }
private:
    void OpenWindow();
    void PullEvents();
    void OnUpdate();
    void OnRender();

private:
    Window mMainWindow;
    SDL_Event mEvent; // want to make an event queuing
    bool mNeedQuit = false;

    TextureManager mTexMgr;
    AnimationManager mAnimMgr;

    Loader mLoader;

    FilmScene mScene;
    Clock mClock;

    size_t mCount = 0;
};
