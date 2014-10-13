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
#include <glm.hpp>
#include <SDL.h>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include <immintrin.h>
#include "SIMD_SSE.h"
#include "camera.h"

/* error reporting function */
void error_handler(const RTCError code)
{
    printf("Embree: ");
    switch (code) {
        case RTC_UNKNOWN_ERROR: printf("RTC_UNKNOWN_ERROR"); break;
        case RTC_INVALID_ARGUMENT: printf("RTC_INVALID_ARGUMENT"); break;
        case RTC_INVALID_OPERATION: printf("RTC_INVALID_OPERATION"); break;
        case RTC_OUT_OF_MEMORY: printf("RTC_OUT_OF_MEMORY"); break;
        case RTC_UNSUPPORTED_CPU: printf("RTC_UNSUPPORTED_CPU"); break;
        default: printf("invalid error code"); break;
    }
    abort();
}
/* vertex and triangle layout */
struct Vertex { float x, y, z, r; };
struct Triangle { int v0, v1, v2; };

#undef main
int main(int argc, char* argv[])
{

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    rtcInit(NULL);

    RTCScene scene = rtcNewScene(RTC_SCENE_STATIC | RTC_SCENE_COHERENT, RTC_INTERSECT1);

    /* create a triangulated cube with 12 triangles and 8 vertices */
    unsigned int mesh = rtcNewTriangleMesh(scene, RTC_GEOMETRY_STATIC, 12, 8);

    /* set vertices */
    Vertex* vertices = (Vertex*) rtcMapBuffer(scene, mesh, RTC_VERTEX_BUFFER);
    vertices[0].x = -1; vertices[0].y = -1; vertices[0].z = -1;
    vertices[1].x = -1; vertices[1].y = -1; vertices[1].z = +1;
    vertices[2].x = -1; vertices[2].y = +1; vertices[2].z = -1;
    vertices[3].x = -1; vertices[3].y = +1; vertices[3].z = +1;
    vertices[4].x = +1; vertices[4].y = -1; vertices[4].z = -1;
    vertices[5].x = +1; vertices[5].y = -1; vertices[5].z = +1;
    vertices[6].x = +1; vertices[6].y = +1; vertices[6].z = -1;
    vertices[7].x = +1; vertices[7].y = +1; vertices[7].z = +1;
    rtcUnmapBuffer(scene, mesh, RTC_VERTEX_BUFFER);

    /* create triangle color array */

    /* set triangles and colors */
    int tri = 0;
    Triangle* triangles = (Triangle*) rtcMapBuffer(scene, mesh, RTC_INDEX_BUFFER);

    // left side
    triangles[tri].v0 = 0; triangles[tri].v1 = 2; triangles[tri].v2 = 1; tri++;
   triangles[tri].v0 = 1; triangles[tri].v1 = 2; triangles[tri].v2 = 3; tri++;

    // right side
    triangles[tri].v0 = 4; triangles[tri].v1 = 5; triangles[tri].v2 = 6; tri++;
    triangles[tri].v0 = 5; triangles[tri].v1 = 7; triangles[tri].v2 = 6; tri++;

    // bottom side
    triangles[tri].v0 = 0; triangles[tri].v1 = 1; triangles[tri].v2 = 4; tri++;
    triangles[tri].v0 = 1; triangles[tri].v1 = 5; triangles[tri].v2 = 4; tri++;

    // top side
    triangles[tri].v0 = 2; triangles[tri].v1 = 6; triangles[tri].v2 = 3; tri++;
    triangles[tri].v0 = 3; triangles[tri].v1 = 6; triangles[tri].v2 = 7; tri++;

    // front side
    triangles[tri].v0 = 0; triangles[tri].v1 = 4; triangles[tri].v2 = 2; tri++;
    triangles[tri].v0 = 2; triangles[tri].v1 = 4; triangles[tri].v2 = 6; tri++;

    // back side
    triangles[tri].v0 = 1; triangles[tri].v1 = 3; triangles[tri].v2 = 5; tri++;
    triangles[tri].v0 = 3; triangles[tri].v1 = 7; triangles[tri].v2 = 5; tri++;

    rtcUnmapBuffer(scene, mesh, RTC_INDEX_BUFFER);


    rtcExit();
    SDL_Quit();
    return 0;
}