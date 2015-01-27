#ifndef __CAMERA_H
#define __CAMERA_H
#include <embree2\rtcore.h>
#include <embree2\rtcore_ray.h>
#include <glm/glm.hpp>
using namespace glm;

namespace embRT
{
    struct Camera
    {
        Camera::Camera(const vec3& position, const vec3& rot, float fov);

        RTCRay GetRay(const int x, const int y) const;
        RTCRay4 GetRayPacket4(const int x, const int y) const;

        vec3 m_TopLeft, m_TopRight, m_DownLeft;
        vec3 m_FrontDir, m_RightDir, m_UpDir;
        vec3 m_Position;
        vec3 m_Rotation;
        mat4 m_RotationMatrix;
        float m_Fov;
    };
} // namespace embRT

#endif // __CAMERA_H