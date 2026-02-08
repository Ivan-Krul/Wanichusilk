#include "Logger.h"

Logger::Logger(const char* logname) {
    mFoutStream.open(logname, std::ios::app | std::ios::out);
}

#ifdef ENABLE_CONSOLE_LOG
#define _LOGGER_GENERATE_LOG_PRINT(PREFIX) printf("[%s] %s: %s\n", mTimeBuffer, PREFIX, mTextBuffer)
#define _LOGGER_GENERATE_LOG_PRINT_IN(PREFIX, FUNCTION) printf("[%s] (%s) %s: %s\n", mTimeBuffer, FUNCTION, PREFIX, mTextBuffer)
#else
#define _LOGGER_GENERATE_LOG_PRINT(PREFIX)
#define _LOGGER_GENERATE_LOG_PRINT_IN(PREFIX, FUNCTION)
#endif

#ifdef COMPILER_MSVC
#define _LOGGER_LOCALTIME_WRITE(timet) \
    tm timeinfo;\
    localtime_s(&timeinfo, &timet);\
    strftime(mTimeBuffer, sizeof(mTimeBuffer), "%Y.%m.%d %H:%M:%S", &timeinfo)
#else
#define _LOGGER_LOCALTIME_WRITE(timet)\
    tm* timeinfo = std::localtime(&timet);\
    strftime(mTimeBuffer, sizeof(mTimeBuffer), "%Y.%m.%d %H:%M:%S", timeinfo)
#endif

#define _LOGGER_GENERATE_LOG_FUNC \
    assert(sizeof(text) <= 128);\
\
    va_list args;\
    va_start(args, text);\
    vsnprintf(mTextBuffer, 256, text, args);\
    va_end(args);\
\
    time_t now = time(0);\
    _LOGGER_LOCALTIME_WRITE(now);\


void Logger::logDebug(const char* text, ...) {
#ifdef ENABLE_DEBUG_LOG
    _LOGGER_GENERATE_LOG_FUNC;
    mFoutStream << "[" << mTimeBuffer << "] DEBUG" << ": " << mTextBuffer << std::endl;

#ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT("DEBUG");
#endif
#endif
}

void Logger::logDebugIn(const char* function, const char* text, ...) {
#ifdef ENABLE_DEBUG_LOG
    _LOGGER_GENERATE_LOG_FUNC;

    mFoutStream << "[" << mTimeBuffer << "] (" << function << ") DEBUG: " << mTextBuffer << std::endl;

#ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT_IN("DEBUG", function);
#endif
#endif
}

void Logger::logInfo(const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;
    mFoutStream << "[" << mTimeBuffer << "] INFO: " << mTextBuffer << std::endl;

#ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT("INFO");
#endif
}

void Logger::logInfoIn(const char* function, const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;

    mFoutStream << "[" << mTimeBuffer << "] (" << function << ") INFO: " << mTextBuffer << std::endl;

#ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT_IN("INFO", function);
#endif
}

void Logger::logWarning(const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;
    mFoutStream << "[" << mTimeBuffer << "] WARNING: " << mTextBuffer << std::endl;

#ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT("WARNING");
#endif
}

void Logger::logWarningIn(const char* function, const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;

    mFoutStream << "[" << mTimeBuffer << "] (" << function << ") WARNING: " << mTextBuffer << std::endl;

#ifdef DEBUG
    _LOGGER_GENERATE_LOG_PRINT_IN("WARNING", function);
#endif
}

void Logger::logError(const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;
    mFoutStream << "[" << mTimeBuffer << "] ERROR: " << mTextBuffer << std::endl;

    _LOGGER_GENERATE_LOG_PRINT("ERROR");
}

void Logger::logErrorIn(const char* function, const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;

    mFoutStream << "[" << mTimeBuffer << "] (" << function << ") ERROR: " << mTextBuffer << std::endl;

    _LOGGER_GENERATE_LOG_PRINT_IN("ERROR", function);
}

void Logger::logCritical(const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;
    mFoutStream << "[" << mTimeBuffer << "] CRITICAL: " << mTextBuffer << std::endl;

    _LOGGER_GENERATE_LOG_PRINT("CRITICAL");
}

void Logger::logCriticalIn(const char* function, const char* text, ...) {
    _LOGGER_GENERATE_LOG_FUNC;

    mFoutStream << "[" << mTimeBuffer << "] (" << function << ") CRITICAL: " << mTextBuffer << std::endl;

    _LOGGER_GENERATE_LOG_PRINT_IN("CRITICAL", function);
}

Logger::~Logger() {
    mFoutStream.close();
}

void logSDLErr(const char* function) {
    Logger log(DEFAULT_LOG_SDL_PATH);
    log.logErrorIn(function, "%s.", SDL_GetError());
}

void logSDLWrn(const char* function) {
    Logger log(DEFAULT_LOG_SDL_PATH);
    log.logWarningIn(function, "%s.", SDL_GetError());
}
