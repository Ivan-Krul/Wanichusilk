#pragma once
#include "Logger.h"
#include <SDL3/SDL.h>

inline void logSDLErr(const char* function) {
    Logger log(DEFAULT_LOG_SDL_PATH);
    log.logErrorIn(function, "%s.", SDL_GetError());
}

inline void logSDLWrn(const char* function) {
    Logger log(DEFAULT_LOG_SDL_PATH);
    log.logWarningIn(function, "%s.", SDL_GetError());
}
