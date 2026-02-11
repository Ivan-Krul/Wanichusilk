#ifndef DEFINE_H
#define DEFINE_H
#include <cassert>

#define DEBUG

#define DEFAULT_SCR_RES_X 640
#define DEFAULT_SCR_RES_Y 480

#define POWER_OF_TWO(n) (1 << (n))
#define LOCKER_BIT_SIZE 4
#define LOCKER_SECTION_SIZE POWER_OF_TWO(LOCKER_BIT_SIZE)

#define DEFAULT_RESBITSET_C_BYTES 4
#define DEFAULT_RESBITSET_SIZE_LIM 0x2000
#define DEFAULT_RESBITSET_INDEX_TYPE int

#define DEFAULT_ANIM_SLOT_USE_THRESHOLD 100 * 100 * 10
#define DEFAULT_ANIM_SLOT_SIZE 4

#define DEFAULT_LOG_SDL_PATH "./SDL3.log"
#define DEFAULT_LOG_PATH "./Wanichusilk.log"

#if __cplusplus >= 201703L
#define _FALLTHROUGH [[fallthrough]]
#else
#define _FALLTHROUGH
#endif

#ifndef _NODISCARD
#if __cplusplus >= 201103L
#define _NODISCARD [[nodiscard]]
#else
#define _NODISCARD
#endif
#endif

#if __cplusplus >= 201703L
#define _MAYBE_UNUSED [[maybe_unused]]
#else
#define _MAYBE_UNUSED
#endif

#ifdef _WIN32
#define OS_WINDOWS
#elif __linux__
#define OS_LINUX
#elif __APPLE__
#define OS_MACOS
#elif __ANDROID__
#define OS_ANDROID
#elif __FreeBSD__
#define OS_FREEBSD
#elif __unix__
#define OS_UNIX
#else
#define OS_UNDEFINED
#endif

#ifdef _MSC_VER
#define COMPILER_MSVC
// bad practice, but I know what what should I expect
#pragma warning( disable : 4244 ) 
#pragma warning( disable : 4267 ) 
#endif

#ifdef __cplusplus 
#define NON_COPYABLE(ClassName)                  \
    ClassName(const ClassName&) = delete;        \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) noexcept = default;   \
    ClassName& operator=(ClassName&&) noexcept = default;
#endif

#define ENABLE_DEBUG_LOG

//#define USE_THIRD_PARTY_MATH
//#define NO_CLOCK_TRACKING
#define PLACE_ANIMATION_AS_IMAGE
#define ENABLE_DEBUG_LOG
#define ENABLE_CONSOLE_LOG
#define ENABLE_LOG_ABORT

#endif
