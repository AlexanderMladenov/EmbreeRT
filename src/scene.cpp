#include "scene.h"
#include "objReader.h"
namespace embRT
{
    Scene::Scene() :
        camera(std::make_unique<Camera>(vec3(0, 3, -7), vec3(20, 0, 0), 100)),
        rtcscene(rtcNewScene(RTC_SCENE_STATIC, RTC_INTERSECT1))
    {
    }

    void Scene::addPlane()
    {
        ///* create a triangulated plane with 2 triangles and 4 vertices */
        unsigned int plane = rtcNewTriangleMesh(scene->rtcscene, RTC_GEOMETRY_STATIC, 2, 4);

        /* set vertices */
        Vertex* v = (Vertex*) rtcMapBuffer(scene->rtcscene, plane, RTC_VERTEX_BUFFER);
        v[0].x = -10; v[0].y = -0.5; v[0].z = -10;
        v[1].x = -10; v[1].y = -0.5; v[1].z = +10;
        v[2].x = +10; v[2].y = -0.5; v[2].z = -10;
        v[3].x = +10; v[3].y = -0.5; v[3].z = +10;
        rtcUnmapBuffer(scene->rtcscene, plane, RTC_VERTEX_BUFFER);

        /* set triangles */
        Triangle* tris = (Triangle*) rtcMapBuffer(scene->rtcscene, plane, RTC_INDEX_BUFFER);
        tris[0].v[0] = 0; tris[0].v[1] = 2; tris[0].v[2] = 1;
        tris[1].v[0] = 1; tris[1].v[1] = 2; tris[1].v[2] = 3;
        rtcUnmapBuffer(scene->rtcscene, plane, RTC_INDEX_BUFFER);
    }
    Scene* scene = nullptr;
}