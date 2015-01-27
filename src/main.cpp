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
#include "scene.h"

#include "renderer.h"
#include "util.h"
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

using namespace embRT;
#undef main
int main(int argc, char* argv[])
{
    rtcInit(nullptr);
    rtcSetErrorFunction(error_handler);

    if (SDL_Init(SDL_INIT_EVERYTHING != 0))
    {
        std::cout << "SDL_init failed: " << SDL_GetError() << std::endl;
        throw std::runtime_error("SDL_Init() failed !");
    }

    SDLRenderer* render = new SDLRenderer();
    scene = new Scene();
    //Mesh m;
    ////m.m_Data = readOBJ("../resources/sponza.obj");
    //m.m_Data = readOBJ("../resources/teapot_hires.obj");

    //auto& verts = m.GenerateVertexBufferAligned();

    //auto trisCount = std::get<3>(m.m_Data).size();
    //auto vertsCount = verts.size();
    //auto indBuf = m.GenerateIndexBufferAligned();

    //unsigned int mesh = rtcNewTriangleMesh(scene, RTC_GEOMETRY_STATIC, trisCount, vertsCount);

    //Vertex* vertices = (Vertex*) rtcMapBuffer(scene, mesh, RTC_VERTEX_BUFFER);
    //memcpy(vertices, verts.data(), vertsCount * sizeof(Vertex));
    //rtcUnmapBuffer(scene, mesh, RTC_VERTEX_BUFFER);

    //Triangle* triangles = (Triangle*) rtcMapBuffer(scene, mesh, RTC_INDEX_BUFFER);
    //memcpy(triangles, indBuf.data(), trisCount * sizeof(Triangle));
    //rtcUnmapBuffer(scene, mesh, RTC_INDEX_BUFFER);
    scene->addPlane();
    scene->CommitRTCScene();

    //AreaLight light;
    //light.transform.translate(vec3(10, 10, 10)); // teapot
    ////light.transform.translate(vec3(-300, 700, -5)); // sponza
    //// do not swap with translate, fucks up soft shadows
    //light.transform.scale(vec3(10));

    //light.init();

    //Camera cam(vec3(0, 3, -7), vec3(20, 0, 0), 100); // teapot
    //Camera cam(vec3(0, 30, -7), vec3(20, 90, 0), 100); // dust 2
    //Camera cam(vec3(-200, 100, -5), vec3(0, 270, 0), 100); // sponza

    auto t1 = std::chrono::high_resolution_clock::now();
    //RenderToBufferThreaded(cam, FrameBuf, scene, m, light);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto time = timePast(t1, t2);
    auto time2 = timePast<std::chrono::milliseconds>(t1, t2);
    auto ms = time2 - (time * 1000);
  //  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
   /* if (!InitVideo())
    {
        rtcExit();
        SDL_Quit();
        return -1;
    }
    SwapBuffers(FrameBuf);*/

    std::stringstream ss;
    ss << "embRT: " << time << " s " << ms << " ms" << std::endl;
    //SDL_SetWindowTitle(m_Window, ss.str().c_str());
    std::cout << "Rendering took " << time << " s " << ms << " ms" << std::endl;

    //WaitForUserExit();
    delete scene;
    rtcExit();
    delete render;
    return 0;
}