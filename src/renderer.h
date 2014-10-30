/* The MIT License(MIT)
*
* Copyright(c) 2014 Alexander Mladenov
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE. */

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

    void SwapBuffers(const std::array<std::array<vec3, FRAME_HEIGHT>, FRAME_WIDTH> & buf)
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

    std::array<vec3, 4> PathTrace4(RTCRay4& rays, const vec3& pathMultiplier, int depth, const RTCScene& scene, const Mesh& data)
    {
        std::array<vec3, 4> resultGi;
        if (depth == MAX_TRACE_DEPTH)
        {
            return resultGi;
        }

        __m128i traceMask = _mm_set1_epi32(0xFFFFFFFF);
        rtcIntersect4(&traceMask, scene, rays);

        auto& normals = std::get<1>(data.m_Data);
        auto& triangles = std::get<3>(data.m_Data);

        for (auto i = 0; i < 4; i++)
        {
            if (rays.geomID[i] != RTC_INVALID_GEOMETRY_ID)
            {
                auto& tri = triangles[rays.primID[i]];
                if (normals.size() > 1)
                {
                    auto n0 = normals[tri.n[0]];
                    auto n1 = normals[tri.n[1]];
                    auto n2 = normals[tri.n[2]];
                    float u = rays.u[i], v = rays.v[i], w = 1.0f - rays.u[i] - rays.v[i];
                    vec3 Ns = w * n0 + u * n1 + v * n2;
                    normalize(Ns);

                    RTCRay4 w_out;
                    vec3 brdfEval; // brdf at the chosen direction
                    float pdf; // the probability to choose that specific newRay

                    //// sample the BRDF:
                    //closestNode->shader->spawnRay(rays, w_out, brdfEval, pdf);

                    //if (pdf < 0) return Color(1, 0, 0);  // bogus BRDF; mark in red
                    //if (pdf == 0) return Color(0, 0, 0);  // terminate the path, as required
                    //resultGi = PathTrace4(w_out, pathMultiplier * brdfEval / pdf, depth + 1, scene); // continue the path normally; accumulate the new term to the BRDF product
                }
            }
        }

        return  resultGi;

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