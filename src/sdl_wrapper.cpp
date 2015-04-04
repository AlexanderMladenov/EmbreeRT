#include "sdl_wrapper.h"
#include "util.h"
#include "constants.h"
#include "renderer.h"

#include <stdexcept>

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

    std::unique_ptr<SDLRendering> sdlRenderer;

    SDLRendering::SDLRendering()
    {
        auto begin = std::chrono::high_resolution_clock::now();

        sdlWindow = SDL_CreateWindow("embrt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     FRAME_WIDTH, FRAME_HEIGHT, SDL_WINDOW_OPENGL);

        if (!sdlWindow)
        {
            printf("SDL_CreateWindow failed: %s \n", SDL_GetError());
            throw std::runtime_error("SDL_CreateWindow failed !");
        }

        sdlWindowSurface = SDL_GetWindowSurface(sdlWindow);

        if (!sdlWindowSurface)
        {
            printf("SDL_GetWindowSurface failed: %s \n", SDL_GetError());
            throw std::runtime_error("SDL_GetWindowSurface failed!");
        }

        m_redShift = sdlWindowSurface->format->Rshift;
        m_greenShift = sdlWindowSurface->format->Gshift;
        m_blueShift = sdlWindowSurface->format->Bshift;

        auto end = std::chrono::high_resolution_clock::now();

        auto timeSec = timePast(begin, end);
        auto timeMili = timePast<std::chrono::milliseconds>(begin, end);

        printf("Seting up SDL took: %Iu sec %Iu milisec \n",
               timeSec, timeMili - (timeSec * 1000));
    }

    SDLRendering::~SDLRendering()
    {
        SDL_DestroyWindow(sdlWindow);
        SDL_DestroyRenderer(sdlRenderer);
    }

    Uint32 SDLRendering::ConvertPixel(float pr, float pg, float pb)
    {
        std::uint8_t r, g, b;
        r = (Uint8)(clamp(pr, 0.f, 1.f) * 255);
        g = (Uint8)(clamp(pg, 0.f, 1.f) * 255);
        b = (Uint8)(clamp(pb, 0.f, 1.f) * 255);

        return (b << m_blueShift) | (g << m_greenShift) | (r << m_redShift);
    }

    void SDLRendering::SwapBuffers()
    {
        for (int y = 0; y < FRAME_HEIGHT; y++)
        {
            Uint32 *row = (Uint32*)((Uint8*)sdlWindowSurface->pixels + y * sdlWindowSurface->pitch);
            for (int x = 0; x < FRAME_WIDTH; x++)
                row[x] = ConvertPixel(frameRed[x][y], frameGreen[x][y], frameBlue[x][y]);
        }
        SDL_UpdateWindowSurface(sdlWindow);
    }
}