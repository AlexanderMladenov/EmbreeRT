#ifndef __SDL_WRAPPER_H
#define __SDL_WRAPPER_H

#include <SDL.h>
#include <cstdio>
// REWORK !!!!!!!!!!!!!!!!!!!
namespace embRT
{
    bool SETUP_SDL();

    class SDLSurface
    {
        public:
        // Constructors
        SDLSurface(SDL_Surface* surface) : m_surface(surface) {}

        SDLSurface(SDLSurface&& surface);

        ~SDLSurface();

        // Operators
        SDLSurface& operator=(SDLSurface&& surface);

        // Getters
        int GetWidth() const { return m_surface->w; }
        int GetHeight() const { return m_surface->h; }

        // Removed
        SDLSurface(const SDLSurface& surface) = delete;
        SDLSurface& operator=(const SDLSurface& surface) = delete;


        private:
        SDL_Surface* m_surface = nullptr;
    };

    class SDLWindow
    {
        public:
        // Constructors
        SDLWindow(SDL_Window* window) : m_window(window) {}

        SDLWindow(SDLWindow&& surface);

        ~SDLWindow();

        // Operators
        SDLWindow& operator=(SDLWindow&& surface);

        // Removed
        SDLWindow() = delete;
        SDLWindow(const SDLWindow& surface) = delete;
        SDLWindow& operator=(const SDLWindow& surface) = delete;

        private:
        SDL_Window* m_window = nullptr;
    };

    class SDLRenderer
    {
        public:
        // Constructors
        SDLRenderer(SDL_Renderer* renderer) : m_renderer(renderer) {}

        SDLRenderer(SDLRenderer&& surface);

        ~SDLRenderer();
        // Operators
        SDLRenderer& operator=(SDLRenderer&& surface);

        // Removed
        SDLRenderer() = delete;
        SDLRenderer(const SDLRenderer& surface) = delete;
        SDLRenderer& operator=(const SDLRenderer& surface) = delete;

        private:
        SDL_Renderer* m_renderer = nullptr;
    };

}
#endif