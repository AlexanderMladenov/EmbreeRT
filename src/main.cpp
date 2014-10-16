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

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    rtcInit(NULL);
    if (!InitVideo())
    {
        rtcExit();
        SDL_Quit();
        return -1;
    }
    rtcSetErrorFunction(error_handler);

    Camera cam(vec3(-9, 6, 0), vec3(90, 0, 0), 75);

    RTCScene scene = rtcNewScene(RTC_SCENE_STATIC | RTC_SCENE_COHERENT, RTC_INTERSECT4);

    /* create a triangulated plane with 2 triangles and 4 vertices */
    unsigned int mesh = rtcNewTriangleMesh(scene, RTC_GEOMETRY_STATIC, 2, 4);

    /* set vertices */
    Vertex* vertices = (Vertex*)rtcMapBuffer(scene, mesh, RTC_VERTEX_BUFFER);
    vertices[0].x = -10; vertices[0].y = -2; vertices[0].z = -10;
    vertices[1].x = -10; vertices[1].y = -2; vertices[1].z = +10;
    vertices[2].x = +10; vertices[2].y = -2; vertices[2].z = -10;
    vertices[3].x = +10; vertices[3].y = -2; vertices[3].z = +10;
    rtcUnmapBuffer(scene, mesh, RTC_VERTEX_BUFFER);

    /* set triangles */
    Triangle* triangles = (Triangle*)rtcMapBuffer(scene, mesh, RTC_INDEX_BUFFER);
    triangles[0].v0 = 0; triangles[0].v1 = 2; triangles[0].v2 = 1;
    triangles[1].v0 = 1; triangles[1].v1 = 2; triangles[1].v2 = 3;
    rtcUnmapBuffer(scene, mesh, RTC_INDEX_BUFFER);
    rtcCommit(scene);

    auto f = readFile("../meshes/teapot_lowres.obj");
    auto a = extractVertices(0, f.size());
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