#pragma once
#include <SDL3/SDL.h>

#include "Window.h"

class Application {
public:
    void OnInit();
    void OnLoop();


    inline bool IsRunning() { return !mNeedQuit; }
private:
    void OpenWindow();
    void PullEvents();

private:
    Window mMainWindow;
    SDL_Event mEvent; // want to make an event queuing
    bool mNeedQuit = false;

};
