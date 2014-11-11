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
        r = (Uint8) (clamp(pixel.r, 0.f, 1.f) * 255);
        g = (Uint8) (clamp(pixel.g, 0.f, 1.f) * 255);
        b = (Uint8) (clamp(pixel.b, 0.f, 1.f) * 255);

        return (b << blueShift) | (g << greenShift) | (r << redShift);
    }

    using namespace glm;

    void SwapBuffers(const std::array<std::array<vec3, FRAME_HEIGHT>, FRAME_WIDTH> & buf)
    {
        for (int y = 0; y < FRAME_HEIGHT; y++)
        {
            Uint32 *row = (Uint32*) ((Uint8*) m_Surface->pixels + y * m_Surface->pitch);
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

    vec3 Raytrace(RTCRay ray, const RTCScene& scene, const Mesh& data, const AreaLight& light)
    {
        vec3 result(0);
        auto& normals = std::get<1>(data.m_Data);
        auto& triangles = std::get<3>(data.m_Data);

        rtcIntersect(scene, ray);
        if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            auto& tri = triangles[ray.primID];
            auto n0 = normals[tri.n[0]];
            auto n1 = normals[tri.n[1]];
            auto n2 = normals[tri.n[2]];

            float u = ray.u;
            float v = ray.v;
            float w = 1.0f - u - v;
            vec3 Ns = w * n0 + u * n1 + v * n2;
            normalize(Ns);
            auto mesh = data;
            result = mesh.m_material.shade(scene, ray, Ns, light);

        }
        return result;
    }

    vec3 PathTrace(RTCRay& ray, const vec3& pathMultiplier, int depth, const RTCScene& scene, const Mesh& data)
    {
        vec3 resultGi;
        if (depth == MAX_TRACE_DEPTH)
        {
            return resultGi;
        }
        static auto& normals = std::get<1>(data.m_Data);
        static auto& triangles = std::get<3>(data.m_Data);
        rtcIntersect(scene, ray);
        if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            auto& tri = triangles[ray.primID];
            auto n0 = normals[tri.n[0]];
            auto n1 = normals[tri.n[1]];
            auto n2 = normals[tri.n[2]];

            float u = ray.u;
            float v = ray.v;
            float w = 1.0f - u - v;
            vec3 Ns = w * n0 + u * n1 + v * n2;
            normalize(Ns);

            RTCRay w_out;
            vec3 brdfEval;
            float pdf = 1;

            Lambert material;

            //material.spawnRay(Ns, ray, w_out, brdfEval, pdf);
            if (pdf == 0) return vec3(0);
            resultGi = PathTrace(w_out, pathMultiplier * brdfEval / pdf, depth + 1, scene, data);
            return resultGi;
        }

        return  resultGi;

    }
    void PathTraceToBuffer(const Camera& cam, std::array<std::array<vec3, FRAME_HEIGHT>, FRAME_WIDTH>& buf, const RTCScene& scene, const Mesh& data)
    {
        std::mt19937 rng(std::time(NULL));
        std::uniform_real_distribution<float> dst;
        for (auto x = 0; x < FRAME_WIDTH; x++)
        {
            for (auto y = 0; y < FRAME_HEIGHT; y++)
            {
                vec3 average(0);
                for (auto i = 0; i < PIXEL_PATHS; i++)
                {
                    average += PathTrace(cam.GetRay(x + dst(rng), y + dst(rng)), vec3(1), 0, scene, data);
                }
                average / (float) PIXEL_PATHS;
                buf[x][y] = average;
                std::cout << x << " " << y << std::endl;
                SwapBuffers(buf);
            }
        }
        SwapBuffers(buf);
    }

    void RenderToBuffer(const Camera& cam, std::array<std::array<vec3, FRAME_HEIGHT>, FRAME_WIDTH>& buf, const RTCScene& scene, const Mesh& data, const AreaLight& light)
    {
        for (auto x = 0; x < FRAME_WIDTH; x++)
        {
            for (auto y = 0; y < FRAME_HEIGHT; y++)
            {
                auto ray = cam.GetRay(x, y);
                buf[x][y] = Raytrace(ray, scene, data, light);
            }
        }
        SwapBuffers(buf);
    }
    std::array<vec3, 4> Raytrace4(RTCRay4& rays, const RTCScene& scene, const Mesh& data)
    {
        std::array<vec3, 4> result;
        __m128i traceMask = _mm_set1_epi32(0xFFFFFFFF);
        auto& normals = std::get<1>(data.m_Data);
        auto& triangles = std::get<3>(data.m_Data);

        rtcIntersect4(&traceMask, scene, rays);

        for (auto i = 0; i < 4; i++)
        {
            if (rays.geomID[i] != RTC_INVALID_GEOMETRY_ID)
            {
                auto& tri = triangles[rays.primID[i]];
                auto n0 = normals[tri.n[0]];
                auto n1 = normals[tri.n[1]];
                auto n2 = normals[tri.n[2]];
                float u = rays.u[i], v = rays.v[i], w = 1.0f - rays.u[i] - rays.v[i];
                vec3 Ns = w * n0 + u * n1 + v * n2;
                normalize(Ns);
                result[3 - i] = (vec3(1.0) + Ns) * vec3(0.5);
                //result[3 - i] = (vec3(1.0) + vec3(rays.Ngx[i], rays.Ngy[i], rays.Ngz[i])) * vec3(0.5);
            }
        }
        return result;
    }



    void RenderToBuffer4(const Camera& cam, std::array<std::array<vec3, FRAME_HEIGHT>, FRAME_WIDTH>& buf, const RTCScene& scene, const Mesh& data)
    {
        for (auto x = 0; x < FRAME_WIDTH; x += 2)
        {
            for (auto y = 0; y < FRAME_HEIGHT; y += 2)
            {
                auto rayP = cam.GetRayPacket4(x, y);
                auto colors = Raytrace4(rayP, scene, data);
                buf[x][y] = colors[3];
                buf[x + 1][y] = colors[2];
                buf[x][y + 1] = colors[1];
                buf[x + 1][y + 1] = colors[0];

            }
        }
        SwapBuffers(buf);
    }


} // namespace embRT
#endif