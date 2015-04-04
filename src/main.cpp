#include <iostream>
#include <chrono>
#include <array>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <random>
#include <ctime>
#include <functional>

#include <SDL.h>
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>

#define GLM_FORCE_RADIANS 
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <immintrin.h>

#include "sdl_wrapper.h"
#include "scene.h"

#include "renderer.h"
#include "util.h"
#include "SIMD_SSE.h"
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
    assert(false);
    exit(-2);
}

using namespace embRT;
#undef main
int main(int argc, char* argv[])
{
    rtcInit(nullptr);
    rtcSetErrorFunction(error_handler);
    atexit(rtcExit);

    if (!SETUP_SDL())
    {
        return -42;
    }
    SDL_version ver;
    SDL_GetVersion(&ver);
    printf("SDL Version: %d.%d.%d \n", ver.major, ver.minor, ver.patch);
    atexit(SDL_Quit);

    //Camera cam(vec3(0, 3, -7), vec3(20, 0, 0), 100); // teapot
    //Camera cam(vec3(0, 30, -7), vec3(20, 90, 0), 100); // dust 2
    //Camera cam(vec3(-200, 100, -5), vec3(0, 270, 0), 100); // sponza

    scene = std::make_unique<Scene>(vec3(0, 3, -7), vec3(20, 0, 0), 90);
    scene->addMeshFromOBJ("../resources/teapot_lowres.obj");

    scene->addPlane(-0.5f);
    scene->CommitRTCScene();

    //AreaLight light;
    //light.transform.translate(vec3(10, 10, 10)); // teapot
    ////light.transform.translate(vec3(-300, 700, -5)); // sponza
    //// do not swap with translate, fucks up soft shadows
    //light.transform.scale(vec3(10));
    //light.init();

    auto t1 = std::chrono::high_resolution_clock::now();
    RenderFrame();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto time = timePast(t1, t2);
    auto time2 = timePast<std::chrono::milliseconds>(t1, t2);
    auto ms = time2 - (time * 1000);
    std::cout << "Rendering took " << time << " s " << ms << " ms" << std::endl;
    sdlRenderer = std::make_unique<SDLRendering>();

    sdlRenderer->SwapBuffers();

    std::stringstream ss;
    ss << "embRT: " << time << " s " << ms << " ms" << std::endl;
    //SDL_SetWindowTitle(m_Window, ss.str().c_str());

    system("pause");
    return 0;
}