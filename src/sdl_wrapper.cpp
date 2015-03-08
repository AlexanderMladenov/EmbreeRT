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

    SDLSurface::SDLSurface(SDLSurface&& surface)
    {
        m_surface = surface.m_surface;
        surface.m_surface = nullptr;
    }

    SDLSurface& SDLSurface::operator =(SDLSurface&& surface)
    {
        if (&surface != this)
        {
            m_surface = surface.m_surface;
            surface.m_surface = nullptr;
        }

        return *this;
    }

    SDLSurface::~SDLSurface()
    {
        if (m_surface)
        {
            SDL_FreeSurface(m_surface);
        }
    }

    SDLWindow::SDLWindow(SDLWindow&& window)
    {
        m_window = window.m_window;
        window.m_window = nullptr;
    }

    SDLWindow& SDLWindow::operator =(SDLWindow&& window)
    {
        if (&window != this)
        {
            m_window = window.m_window;
            window.m_window = nullptr;
        }

        return *this;
    }

    SDLWindow::~SDLWindow()
    {
        if (m_window)
        {
            SDL_DestroyWindow(m_window);
        }
    }
}