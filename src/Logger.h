#pragma once
#include <fstream>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdarg.h>

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

    char mTextBuffer[256];
    char mTimeBuffer[20];
};

inline Logger::Logger(const char* logname) {
    mFoutStream.open(logname, std::ios::app | std::ios::out);
}

#ifdef ENABLE_CONSOLE_LOG
#define _LOGGER_GENERATE_LOG_PRINT(PREFIX) printf("[%s] %s: %s\n", mTimeBuffer, PREFIX, mTextBuffer)
#define _LOGGER_GENERATE_LOG_PRINT_IN(PREFIX, FUNCTION) printf("[%s] (%s) %s: %s\n", mTimeBuffer, FUNCTION, PREFIX, mTextBuffer)
#else
#define _LOGGER_GENERATE_LOG_PRINT(PREFIX)
#define _LOGGER_GENERATE_LOG_PRINT_IN(PREFIX, FUNCTION)
#endif

#define _LOGGER_GENERATE_LOG_FUNC \
    assert(sizeof(text) <= 128);\
\
    va_list args;\
    va_start(args, text);\
    vsprintf_s(mTextBuffer, 256, text, args);\
    va_end(args);\
\
    time_t now = time(0);\
    tm timeinfo;\
    localtime_s(&timeinfo, &now);\
    strftime(mTimeBuffer, sizeof(mTimeBuffer), "%Y.%m.%d %H:%M:%S", &timeinfo)

inline void Logger::logDebug(const char* text, ...) {
#ifdef ENABLE_DEBUG_LOG
    _LOGGER_GENERATE_LOG_FUNC;
    mFoutStream << "[" << mTimeBuffer << "] DEBUG" << ": " << mTextBuffer << std::endl;

    #ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT("DEBUG");
    #endif
#endif
}

inline void Logger::logDebugIn(const char* function, const char* text, ...) {
#ifdef ENABLE_DEBUG_LOG
    _LOGGER_GENERATE_LOG_FUNC;

    mFoutStream << "[" << mTimeBuffer << "] (" << function << ") DEBUG: " << mTextBuffer << std::endl;

    #ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT_IN("DEBUG", function);
    #endif
#endif
}

inline void Logger::logInfo(const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;
    mFoutStream << "[" << mTimeBuffer << "] INFO: " << mTextBuffer << std::endl;

#ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT("INFO");
#endif
}

inline void Logger::logInfoIn(const char* function, const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;

    mFoutStream << "[" << mTimeBuffer << "] (" << function << ") INFO: " << mTextBuffer << std::endl;

#ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT_IN("INFO", function);
#endif
}

inline void Logger::logWarning(const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;
    mFoutStream << "[" << mTimeBuffer << "] WARNING: " << mTextBuffer << std::endl;

#ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT("WARNING");
#endif
}

inline void Logger::logWarningIn(const char* function, const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;

    mFoutStream << "[" << mTimeBuffer << "] (" << function << ") WARNING: " << mTextBuffer << std::endl;

#ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT_IN("WARNING", function);
#endif
}

inline void Logger::logError(const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;
    mFoutStream << "[" << mTimeBuffer << "] ERROR: " << mTextBuffer << std::endl;

    _LOGGER_GENERATE_LOG_PRINT("ERROR");
}

inline void Logger::logErrorIn(const char* function, const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;

    mFoutStream << "[" << mTimeBuffer << "] (" << function << ") ERROR: " << mTextBuffer << std::endl;

    _LOGGER_GENERATE_LOG_PRINT_IN("ERROR", function);
}

inline void Logger::logCritical(const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;
    mFoutStream << "[" << mTimeBuffer << "] CRITICAL: " << mTextBuffer << std::endl;

    _LOGGER_GENERATE_LOG_PRINT("CRITICAL");
}

inline void Logger::logCriticalIn(const char* function, const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;

    mFoutStream << "[" << mTimeBuffer << "] (" << function << ") CRITICAL: " << mTextBuffer << std::endl;

    _LOGGER_GENERATE_LOG_PRINT_IN("CRITICAL", function);
}

inline Logger::~Logger() {
    mFoutStream.close();
}
