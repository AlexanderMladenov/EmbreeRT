#ifndef __RENDERER_H
#define __RENDERER_H

#include <iostream>
#include <stdint.h>
#include <chrono>
#include <stdint.h>
#include <array>

#include <SDL.h>
#include <embree2\rtcore.h>
#include <embree2\rtcore_ray.h>
#define GLM_FORCE_RADIANS 
#include <glm\glm.hpp>

#include "constants.h"
#include "scene.h"

using namespace glm;
namespace embRT
{

    
    void WaitForUserExit();
        
    //
    //    vec3 Raytrace(RTCRay ray, const RTCScene& scene, const Mesh& data, const AreaLight& light)
    //    {
    //        vec3 result(0);
    //        auto& normals = std::get<1>(data.m_Data);
    //        auto& triangles = std::get<3>(data.m_Data);
    //
    //        rtcIntersect(scene, ray);
    //        if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
    //        {
    //            auto& tri = triangles[ray.primID];
    //            auto n0 = normals[tri.n[0]];
    //            auto n1 = normals[tri.n[1]];
    //            auto n2 = normals[tri.n[2]];
    //
    //            float u = ray.u;
    //            float v = ray.v;
    //            float w = 1.0f - u - v;
    //            vec3 Ns = w * n0 + u * n1 + v * n2;
    //            normalize(Ns);
    //            vec3 geomN = normalize(vec3(ray.Ng[0], ray.Ng[1], ray.Ng[2]));
    //            if (ray.geomID == 1)
    //            {
    //                result = data.m_material.shade(scene, ray, geomN, light);
    //                return result;
    //            }
    //            result = data.m_material.shade(scene, ray, Ns, light);
    //
    //        }
    //        return result;
    //    }
    //
    //    void RenderToBuffer(const Camera& cam, std::array<std::array<vec3, FRAME_HEIGHT>, FRAME_WIDTH>& buf, const RTCScene& scene, const Mesh& data, const AreaLight& light)
    //    {
    //        for (auto x = 0; x < FRAME_WIDTH; x++)
    //        {
    //            for (auto y = 0; y < FRAME_HEIGHT; y++)
    //            {
    //                auto ray = cam.GetRay(x, y);
    //                buf[x][y] = Raytrace(ray, scene, data, light);
    //            }
    //        }
    //    }
    //
    //    void renderReginon(const Camera& cam, std::array<std::array<vec3, FRAME_HEIGHT>, FRAME_WIDTH>& buf,
    //        const RTCScene& scene, const Mesh& data, const AreaLight& light, int xFrom, int xTo)
    //    {
    //        for (auto x = xFrom; x < xTo; x++)
    //        {
    //            for (auto y = 0; y < FRAME_HEIGHT; y++)
    //            {
    //                auto ray = cam.GetRay(x, y);
    //                buf[x][y] = Raytrace(ray, scene, data, light);
    //            }
    //        }
    //        return;
    //    }
    //
    //    void RenderToBufferThreaded(const Camera& cam, std::array<std::array<vec3, FRAME_HEIGHT>, FRAME_WIDTH>& buf,
    //        const RTCScene& scene, const Mesh& data, const AreaLight& light)
    //    {
    //        auto numThreads = std::thread::hardware_concurrency();
    //        int delta = FRAME_WIDTH / numThreads;
    //        std::vector <std::thread> threads;
    //        for (int i = 0; i < numThreads; i++)
    //        {
    //            auto fun = std::bind(&renderReginon,cam, std::ref(buf), std::ref(scene), std::ref(data), light, delta * i, delta * i + delta);
    //            threads.emplace_back(fun);
    //        }
    //        for (int i = 0; i < numThreads; i++)
    //        {
    //        threads[i].join();
    //        }
    //        return;
    //    }
    //
    //    std::array<vec3, 4> Raytrace4(RTCRay4& rays, const RTCScene& scene, const Mesh& data)
    //    {
    //        std::array<vec3, 4> result;
    //        __m128i traceMask = _mm_set1_epi32(0xFFFFFFFF);
    //        auto& normals = std::get<1>(data.m_Data);
    //        auto& triangles = std::get<3>(data.m_Data);
    //
    //        rtcIntersect4(&traceMask, scene, rays);
    //
    //        for (auto i = 0; i < 4; i++)
    //        {
    //            if (rays.geomID[i] != RTC_INVALID_GEOMETRY_ID)
    //            {
    //                auto& tri = triangles[rays.primID[i]];
    //                auto n0 = normals[tri.n[0]];
    //                auto n1 = normals[tri.n[1]];
    //                auto n2 = normals[tri.n[2]];
    //                float u = rays.u[i], v = rays.v[i], w = 1.0f - rays.u[i] - rays.v[i];
    //                vec3 Ns = w * n0 + u * n1 + v * n2;
    //                normalize(Ns);
    //                result[3 - i] = (vec3(1.0) + Ns) * vec3(0.5);
    //                //result[3 - i] = (vec3(1.0) + vec3(rays.Ngx[i], rays.Ngy[i], rays.Ngz[i])) * vec3(0.5);
    //            }
    //        }
    //        return result;
    //    }
    //
    //
    //
    //    void RenderToBuffer4(const Camera& cam, std::array<std::array<vec3, FRAME_HEIGHT>, FRAME_WIDTH>& buf, const RTCScene& scene, const Mesh& data)
    //    {
    //        for (auto x = 0; x < FRAME_WIDTH; x += 2)
    //        {
    //            for (auto y = 0; y < FRAME_HEIGHT; y += 2)
    //            {
    //                auto rayP = cam.GetRayPacket4(x, y);
    //                auto colors = Raytrace4(rayP, scene, data);
    //                buf[x][y] = colors[3];
    //                buf[x + 1][y] = colors[2];
    //                buf[x][y + 1] = colors[1];
    //                buf[x + 1][y + 1] = colors[0];
    //
    //            }
    //        }
    //        SwapBuffers(buf);
    //    }
    //

} // namespace embRT
#endif