#include "renderer.h"
#include "constants.h"
#include "util.h"
#include "scene.h"

namespace embRT
{
    SDLRenderer::SDLRenderer()
    {
        auto begin = std::chrono::high_resolution_clock::now();

        m_window = SDL_CreateWindow("embrt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    FRAME_WIDTH, FRAME_HEIGHT, SDL_WINDOW_OPENGL);

        if (!m_window)
        {
            std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
            throw std::runtime_error("SDL_CreateWindow failed !");
        }

        m_surface = SDL_GetWindowSurface(m_window);

        if (!m_surface)
        {
            std::cout << "SDL_GetWindowSurface failed: " << SDL_GetError() << std::endl;
            throw std::runtime_error("SDL_GetWindowSurface failed!");
        }

        m_redShift = m_surface->format->Rshift;
        m_greenShift = m_surface->format->Gshift;
        m_blueShift = m_surface->format->Bshift;

        auto end = std::chrono::high_resolution_clock::now();

        auto timeSec = timePast(begin, end);
        auto timeMili = timePast<std::chrono::milliseconds>(begin, end);
        std::cout << "Seting up SDL took: " << timeSec << " sec " << timeMili - (timeSec * 1000) << " milisec" << std::endl;
    }

    SDLRenderer::~SDLRenderer()
    {
        SDL_Quit();
    }

    std::uint32_t SDLRenderer::ConvertPixel(const vec3& pixel) const
    {
        std::uint8_t r, g, b;
        r = (Uint8) (clamp(pixel.r, 0.f, 1.f) * 255);
        g = (Uint8) (clamp(pixel.g, 0.f, 1.f) * 255);
        b = (Uint8) (clamp(pixel.b, 0.f, 1.f) * 255);

        return (b << m_blueShift) | (g << m_greenShift) | (r << m_redShift);
    }

    void SDLRenderer::SwapBuffers()
    {

        for (int y = 0; y < FRAME_HEIGHT; y++)
        {
            Uint32 *row = (Uint32*) ((Uint8*) m_surface->pixels + y * m_surface->pitch);
            for (int x = 0; x < FRAME_WIDTH; x++)
                row[x] = ConvertPixel(m_buffer[x][y]);
        }
        SDL_UpdateWindowSurface(m_window);
    }

   /* vec3 RayTracer::Raytrace(RTCRay& ray)
    {
        vec3 result(0);

        rtcIntersect(scene.rtcscene, ray);
        if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            vec3 geomN = normalize(vec3(ray.Ng[0], ray.Ng[1], ray.Ng[2]));
            result = geomN;

        }
        return result;
    }*/
}