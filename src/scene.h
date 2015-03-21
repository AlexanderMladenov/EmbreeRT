#ifndef __SCENE_H
#define __SCENE_H

#include <memory>
#include <string>

#include <embree2\rtcore.h>
#include <embree2\rtcore_ray.h>

#include "camera.h"
#include "mesh.h"

namespace embRT
{
    struct Scene
    {
        Camera camera;
        RTCScene rtcscene;

        std::vector<Mesh> meshes;

        bool isCommited = false;


        Scene();
        Scene(const vec3& cameraPos, const vec3& cameraRot, float cameraFoV);

        void addPlane(float y);
        void addMeshFromOBJ(const std::string& path);

        inline void CommitRTCScene()
        {
            rtcCommit(rtcscene);
            isCommited = true;
        }

    };

    extern std::unique_ptr<Scene> scene;
}

#endif