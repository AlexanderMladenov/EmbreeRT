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

#include <iostream>
#include <chrono>
#include <array>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>

#include <SDL.h>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

#define GLM_FORCE_RADIANS 
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include <immintrin.h>

#include "constants.h"
#include "SIMD_SSE.h"

#include "util.h"
#include "camera.h"
#include "objReader.h"
#include "renderer.h"


/* error reporting function */
void error_handler(const RTCError code, const char* str)
{
    printf("Embree: ");
    switch (code)
    {
    case RTC_UNKNOWN_ERROR: printf("RTC_UNKNOWN_ERROR"); break;
    case RTC_INVALID_ARGUMENT: printf("RTC_INVALID_ARGUMENT"); break;
    case RTC_INVALID_OPERATION: printf("RTC_INVALID_OPERATION"); break;
    case RTC_OUT_OF_MEMORY: printf("RTC_OUT_OF_MEMORY"); break;
    case RTC_UNSUPPORTED_CPU: printf("RTC_UNSUPPORTED_CPU"); break;
    default: printf("invalid error code"); break;
    }
    exit(-2);
}
namespace embRT{
    std::array<std::array<vec3, FRAME_HEIGHT>, FRAME_WIDTH> FrameBuf;
}

using namespace embRT;
#undef main
int main(int argc, char* argv[])
{
    rtcInit(NULL);
    rtcSetErrorFunction(error_handler);

    auto lines = readOBJ("../meshes/banshi_1.obj");
    auto objData = extractData(0, lines.size());

    auto& verts = std::get<0>(objData);
    auto& tris = std::get<3>(objData);
    auto trisCount = tris.size();
    auto vertsCount = verts.size();
    RTCScene scene = rtcNewScene(RTC_SCENE_STATIC | RTC_SCENE_COHERENT, RTC_INTERSECT4);
    unsigned int mesh = rtcNewTriangleMesh(scene, RTC_GEOMETRY_STATIC, trisCount, vertsCount);

    /*set vertices */
    Vertex* vertices = (Vertex*)rtcMapBuffer(scene, mesh, RTC_VERTEX_BUFFER);
    for (auto j = 0; j < vertsCount; j++)
    {
        vertices[j].x = verts[j].x;
        vertices[j].y = verts[j].y;
        vertices[j].z = verts[j].z;
    }
    rtcUnmapBuffer(scene, mesh, RTC_VERTEX_BUFFER);

    Triangle* triangles = (Triangle*)rtcMapBuffer(scene, mesh, RTC_INDEX_BUFFER);
    for (auto j = 0; j < trisCount; j++)
    {
        triangles[j].v[0] = tris[j].v[0];
        triangles[j].v[1] = tris[j].v[1];
        triangles[j].v[2] = tris[j].v[2];
    }
    rtcUnmapBuffer(scene, mesh, RTC_INDEX_BUFFER);
    rtcCommit(scene);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (!InitVideo())
    {
        rtcExit();
        SDL_Quit();
        return -1;
    }

    Camera cam(vec3(-250, 90, 0), vec3(0, 90, 0), 100);
    auto t1 = std::chrono::high_resolution_clock::now();
    RenderToBuffer4(cam, FrameBuf, scene);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto time = timePast(t1, t2);
    auto time2 = timePast<std::chrono::milliseconds>(t1, t2);
    std::cout << "Rendering took " << time << " sec " << time2 << " milsec" << std::endl;

    WaitForUserExit();
    rtcExit();
    SDL_Quit();
    return 0;
}