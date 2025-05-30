#include "Application.h"

void Application::OnInit() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // the loading of SDL was failed: log
        exit(1);
    }

    OpenWindow();
}

void Application::OnLoop() {
    PullEvents();
    SDL_Delay(10);
}

void Application::OpenWindow() {
    mMainWindow.create("Wanichusilk", 640, 480);
}

void Application::PullEvents() {
    while (SDL_PollEvent(&mEvent)) {
        switch (mEvent.type) {
        case SDL_EVENT_QUIT:
            mNeedQuit = true;
            break;
        default:
            break;
        }
    }
}
