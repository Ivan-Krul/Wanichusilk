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

#if __cplusplus >= 201703L
#define _FALLTHROUGH [[fallthrough]]
#else
#define _FALLTHROUGH
#endif

//#define USE_THIRD_PARTY_MATH
//#define NO_CLOCK_TRACKING

#endif