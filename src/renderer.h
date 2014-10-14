#ifndef __RENDERER_H
#define __RENDERER_H

namespace embRT
{
    SDL_Window* m_Window = nullptr;
    SDL_Surface* m_Surface = nullptr;

    Uint8 redShift;
    Uint8 greenShift;
    Uint8 blueShift;

    std::uint32_t ConvertPixel(const vec3& pixel)
    {
        std::uint8_t r, g, b;
        r = (Uint8)(clamp(pixel.r, 0.f, 1.f) * 255);
        g = (Uint8)(clamp(pixel.g, 0.f, 1.f) * 255);
        b = (Uint8)(clamp(pixel.b, 0.f, 1.f) * 255);

        return (b << blueShift) | (g << greenShift) | (r << redShift);
    }

    using namespace glm;

    void SwapBuffers(const vec3 buf[FRAME_WIDTH][FRAME_HEIGHT])
    {
        for (int y = 0; y < FRAME_HEIGHT; y++)
        {
            Uint32 *row = (Uint32*)((Uint8*)m_Surface->pixels + y * m_Surface->pitch);
            for (int x = 0; x < FRAME_WIDTH; x++)
                row[x] = ConvertPixel(buf[x][y]);
        }
        SDL_UpdateWindowSurface(m_Window);
    }

    void WaitForUserExit()
    {
        SDL_Event e;
        bool wantExit = false;
        while (!wantExit)
        {
            SDL_PollEvent(&e);

            if (e.type == SDL_QUIT)
            {
                wantExit = true;
                exit(0);
            }
        }
    }


    bool InitVideo()
    {
        auto begin = std::chrono::high_resolution_clock::now();
        if (SDL_Init(SDL_INIT_EVERYTHING != 0))
        {
            std::cout << "SDL_init failed: " << SDL_GetError() << std::endl;
            return false;
        }

        m_Window = SDL_CreateWindow("embrt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            FRAME_WIDTH, FRAME_HEIGHT, SDL_WINDOW_OPENGL);

        if (!m_Window)
        {
            std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
            return false;
        }

        m_Surface = SDL_GetWindowSurface(m_Window);

        if (!m_Surface)
        {
            std::cout << "SDL_GetWindowSurface failed: " << SDL_GetError() << std::endl;
            return false;
        }
        redShift = m_Surface->format->Rshift;
        greenShift = m_Surface->format->Gshift;
        blueShift = m_Surface->format->Bshift;

        auto end = std::chrono::high_resolution_clock::now();

        auto timeSec = timePast(begin, end);
        auto timeMili = timePast<std::chrono::milliseconds>(begin, end);
        std::cout << "Seting up SDL took: " << timeSec << " sec " << timeMili - (timeSec * 1000) << " milisec" << std::endl;
        return true;
    }

    vec3 Raytrace(RTCRay& ray, const RTCScene& scene)
    {
        rtcIntersect(scene, ray);
        if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            return vec3(1, 0.3, 0);
        }
        return vec3(0);
    }

    //__aligned(16) int valid4[4] = { -1, -1, -1, -1 };

    void RenderToBuffer(const Camera& cam, vec3 buf[FRAME_WIDTH][FRAME_HEIGHT], const RTCScene& scene)
    {
        for (auto x = 0; x < FRAME_WIDTH; x++)
        {
            for (auto y = 0; y < FRAME_HEIGHT; y++)
            {
                auto r = cam.GetRay(x, y);
                buf[x][y] = Raytrace(r, scene);
            }
        }

        SwapBuffers(buf);
    }
} // namespace embRT
#endif