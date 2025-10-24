#ifndef RECT_MATH_H
#define RECT_MATH_H
#include <SDL3/SDL.h>

#include "easefunc.h"

inline SDL_FRect lerp_rect(const SDL_FRect& from, const SDL_FRect& to, float t) {
    SDL_FRect rect;
    rect.x = lerp(t, from.x, to.x);
    rect.y = lerp(t, from.y, to.y);
    rect.w = lerp(t, from.w, to.w);
    rect.h = lerp(t, from.h, to.h);
    return rect;
}

#endif