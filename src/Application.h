#pragma once
#include <SDL3/SDL.h>

#include "Window.h"
#include "TextureManager.h"
#include "FilmScene.h"
#include "Clock.h"
#include "define.h"

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
    FilmScene mScene;
    Clock mClock;
};
