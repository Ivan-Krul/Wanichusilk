#ifndef DEFINE_H
#define DEFINE_H
#include <SDL3/SDL.h>

#define DEFAULT_SCR_RES_X 640
#define DEFAULT_SCR_RES_Y 480

#define POWER_OF_TWO(n) (1 << (n))
#define LOCKER_BIT_SIZE 4
#define LOCKER_SECTION_SIZE POWER_OF_TWO(LOCKER_BIT_SIZE)

#define RENDERER SDL_Renderer


#endif