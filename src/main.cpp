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

#include <glm.hpp>
#include <SDL.h>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

#include <immintrin.h>

#include "constants.h"
#include "SIMD_SSE.h"

#include "renderer.h"
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
    if (!initVideo())
    {
        rtcExit();
        SDL_Quit();
        return -1;
    }

    RTCScene scene = rtcNewScene(RTC_SCENE_STATIC | RTC_SCENE_COHERENT, RTC_INTERSECT1);

    waitForUserExit();
    rtcExit();
    SDL_Quit();
    return 0;
}