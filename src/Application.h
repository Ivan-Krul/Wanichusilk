#pragma once
#include <SDL3/SDL.h>

#include "Window.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "define.h"

class Application {
public:
    void OnInit();
    void OnLoop();


    inline bool IsRunning() { return !mNeedQuit; }
private:
    void OpenWindow();
    void PullEvents();
    void OnRender();

private:
    Window mMainWindow;
    SDL_Event mEvent; // want to make an event queuing
    bool mNeedQuit = false;

    ResourceManager mResMgr;
};
