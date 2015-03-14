#ifndef __SCENE_H
#define __SCENE_H

#include <memory>

#include <embree2\rtcore.h>
#include <embree2\rtcore_ray.h>

#include "camera.h"

namespace embRT
{
    struct Scene
    {
        std::unique_ptr<Camera> camera = nullptr;
        bool isCommited = false;
        RTCScene rtcscene;

        Scene();

        void addPlane(float y);
        void addMeshFromOBJ(const std::string& path);
        inline void CommitRTCScene()
        {
            rtcCommit(rtcscene);
            isCommited = true;
        }

    };

    extern Scene* scene;
}

#endif