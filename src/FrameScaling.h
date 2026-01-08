#pragma once
#include <SDL3/SDL.h>

struct FrameSize {
    int width = 0;
    int height = 0;
};

struct ScaleOption {

    void updateFrameSize() { if (p_wind) SDL_GetWindowSize(p_wind, &actual_res.width, &actual_res.height); }
    FrameSize getFrameSize() const { return p_wind ? actual_res : predef_res; }

    ScaleOption() = default;
    inline ScaleOption(FrameSize screen_resolution) { predef_res = screen_resolution; }
    inline ScaleOption(SDL_Window* wnd) {
        p_wind = wnd;
        updateFrameSize();
    }

    FrameSize predef_res;
    FrameSize actual_res;
    SDL_Window* p_wind = nullptr;
};

