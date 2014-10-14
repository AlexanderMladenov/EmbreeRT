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

#ifndef __CAMERA_H
#define __CAMERA_H

using namespace glm;

inline mat4 CreateRotationMatrix(const vec3& rot)
{
    return rotate(radians(rot.x), vec3(1, 0, 0)) *
        rotate(radians(rot.y), vec3(0, 1, 0)) *
        rotate(radians(rot.z), vec3(0, 0, 1));
}

struct Camera
{
    Camera::Camera(const vec3& position, const vec3& rot, float fov) :
    m_Position(position),
    m_Rotation(rot),
    m_Fov(fov)
    {
        auto x = -((float)FRAME_WIDTH / (float)FRAME_HEIGHT);
        auto y = 1.f;

        vec3 corner(x, y, 1);
        vec3 center(0, 0, 1);

        auto scaling = tan(radians(m_Fov * 0.5f)) / (corner - center).length();

        x *= scaling;
        y *= scaling;

        m_RotationMatrix = CreateRotationMatrix(m_Rotation);
        m_TopLeft = vec3(m_RotationMatrix * vec4(x, y, 1, 1));
        m_TopRight = vec3(m_RotationMatrix * vec4(-x, y, 1, 1));
        m_DownLeft = vec3(m_RotationMatrix * vec4(x, -y, 1, 1));

        m_UpDir = vec3(m_RotationMatrix * vec4(0, 1, 0, 1));
        m_RightDir = vec3(m_RotationMatrix * vec4(1, 0, 0, 1));
        m_FrontDir = vec3(m_RotationMatrix * vec4(0, 0, 1, 1));

        m_TopLeft += m_Position;
        m_TopRight += m_Position;
        m_DownLeft += m_Position;
    }

    RTCRay GetRay(const int x, const int y) const
    {
        RTCRay result;
        result.org[0] = m_Position.x;
        result.org[1] = m_Position.y;
        result.org[2] = m_Position.z;

        vec3 target = normalize((m_TopLeft +
            (m_TopRight - m_TopLeft) * (x / (float)FRAME_WIDTH) +
            (m_DownLeft - m_TopLeft) * (y / (float)FRAME_HEIGHT)) - m_Position);

        result.dir[0] = target.x;
        result.dir[1] = target.y;
        result.dir[2] = target.z;
        result.tfar = std::numeric_limits<float>::max();
        result.tnear = 0.f;
        result.time = 0.f;
        result.geomID = RTC_INVALID_GEOMETRY_ID;
        result.primID = RTC_INVALID_GEOMETRY_ID;
        result.mask = -1;

        return result;
    }

    RTCRay4 GetRayPacket4(const int x, const int y) const
    {
        RTCRay4 result;
        auto& pos = m_Position;

        __m128 a = _mm_set1_ps(pos.x);
        __m128 b = _mm_set1_ps(pos.y);
        __m128 c = _mm_set1_ps(pos.z);

        memcpy(result.orgx, &a, sizeof(a));
        memcpy(result.orgy, &b, sizeof(b));
        memcpy(result.orgz, &c, sizeof(c));

        vec3 targets[4];
        for (auto i = 0; i < 4; i++)
        {
            targets[i] = normalize((m_TopLeft +
                (m_TopRight - m_TopLeft) * ((x + i) / (float)FRAME_WIDTH) +
                (m_DownLeft - m_TopLeft) * ((y + i) / (float)FRAME_HEIGHT)) - m_Position);
        }

        for (auto i = 0; i < 4; i++)
        {
            result.dirx[i] = targets[i].x;
            result.diry[i] = targets[i].y;
            result.dirz[i] = targets[i].z;
        }

        for (auto i = 0; i < 4; i++)
        {
            result.tfar[i] = std::numeric_limits<float>::max();
            result.tnear[i] = 0.f;
            result.time[i] = 0.f;
            result.geomID[i] = RTC_INVALID_GEOMETRY_ID;
            result.primID[i] = RTC_INVALID_GEOMETRY_ID;
            result.mask[i] = -1;
        }
        return result;
    }

    vec3 m_TopLeft, m_TopRight, m_DownLeft;
    vec3 m_FrontDir, m_RightDir, m_UpDir;
    vec3 m_Position;
    vec3 m_Rotation;
    mat4 m_RotationMatrix;
    float m_Fov;
};

#endif // __CAMERA_H