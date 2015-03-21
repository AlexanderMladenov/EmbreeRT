#include "sdl_wrapper.h"

namespace embRT
{
    bool SETUP_SDL()
    {
        if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0)
        {
            printf("SDL_Init failed: %s/n", SDL_GetError());
            return false;
        }
        return true;
    }

}