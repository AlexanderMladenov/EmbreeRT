#ifndef __SDL_WRAPPER_H
#define __SDL_WRAPPER_H

#include <SDL.h>
#include <cstdio>
#include <memory>

namespace embRT
{
    bool SETUP_SDL();
    
    
    struct SDLRendering
    {
        SDL_Window* sdlWindow = nullptr;
        SDL_Surface* sdlWindowSurface = nullptr;
        SDL_Renderer* sdlRenderer = nullptr;

        Uint8 m_redShift;
        Uint8 m_greenShift;
        Uint8 m_blueShift;

        SDLRendering();
        ~SDLRendering();

        Uint32 ConvertPixel(float pr, float pg, float pb);
        void SwapBuffers();


    };

    extern std::unique_ptr<SDLRendering> sdlRenderer;
}
#endif