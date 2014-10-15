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

namespace embRT
{
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

            // assign origins
            __m128 positionsX = _mm_set1_ps(pos.x);
            __m128 positionsY = _mm_set1_ps(pos.y);
            __m128 positionsZ = _mm_set1_ps(pos.z);
            memcpy(result.orgx, &positionsX, sizeof(positionsX));
            memcpy(result.orgy, &positionsY, sizeof(positionsY));
            memcpy(result.orgz, &positionsZ, sizeof(positionsZ));

            vec3 targets[4];
            for (auto i = 0; i < 4; i++)
            {
                targets[i] = normalize((m_TopLeft +
                    (m_TopRight - m_TopLeft) * ((x) / (float)FRAME_WIDTH) +
                    (m_DownLeft - m_TopLeft) * ((y + i) / (float)FRAME_HEIGHT)) - m_Position);
            }
            // assign directions
            __m128 directionsX = _mm_set_ps(targets[0].x, targets[1].x, targets[2].x, targets[3].x);
            __m128 directionsY = _mm_set_ps(targets[0].y, targets[1].y, targets[2].y, targets[3].y);
            __m128 directionsZ = _mm_set_ps(targets[0].z, targets[1].z, targets[2].z, targets[3].z);
            memcpy(result.dirx, &directionsX, sizeof(directionsX));
            memcpy(result.diry, &directionsY, sizeof(directionsY));
            memcpy(result.dirz, &directionsZ, sizeof(directionsZ));

            memcpy(result.tfar, &SIMDConstants::infinity, sizeof(SIMDConstants::infinity)); // tfar = inf;
            memcpy(result.tnear, &SIMDConstants::zero, sizeof(SIMDConstants::zero)); // tnear = 0;
            memcpy(result.time, &SIMDConstants::zero, sizeof(SIMDConstants::zero)); // time = 0;

            __m128i geomIDs = _mm_set1_epi32(RTC_INVALID_GEOMETRY_ID);
            memcpy(result.geomID, &geomIDs, sizeof(geomIDs)); // geomIDs = RTC_INVALID_GEOMETRY_ID
            memcpy(result.primID, &geomIDs, sizeof(geomIDs)); // geomIDs = RTC_INVALID_GEOMETRY_ID
            __m128i maskIDs = _mm_set1_epi32(-1);
            memcpy(result.mask, &maskIDs, sizeof(maskIDs)); // geom mask = -1

            return result;
        }

        vec3 m_TopLeft, m_TopRight, m_DownLeft;
        vec3 m_FrontDir, m_RightDir, m_UpDir;
        vec3 m_Position;
        vec3 m_Rotation;
        mat4 m_RotationMatrix;
        float m_Fov;
    };
} // namespace embRT

#endif // __CAMERA_H