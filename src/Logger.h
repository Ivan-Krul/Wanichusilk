#pragma once
#include <fstream>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdarg.h>

#include <SDL3/SDL.h>

#include "define.h"

class Logger {
public:
    Logger(const char* logname);

    void logDebug(const char* text, ...);
    void logDebugIn(const char* function, const char* text, ...);

    void logInfo(const char* text, ...);
    void logInfoIn(const char* function, const char* text, ...);

    void logWarning(const char* text, ...);
    void logWarningIn(const char* function, const char* text, ...);

    void logError(const char* text, ...);
    void logErrorIn(const char* function, const char* text, ...);

    void logCritical(const char* text, ...);
    void logCriticalIn(const char* function, const char* text, ...);

    ~Logger();
private:
    std::ofstream mFoutStream;

    char mTextBuffer[256] = { 0 };
    char mTimeBuffer[20] = { 0 };
};

void logSDLErr(const char* function);
void logSDLWrn(const char* function);
