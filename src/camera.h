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
using namespace Eigen;
struct Camera
{

    Camera(const Vector3f& position, const Vector3f& rot, float fov = 75.f);

    RTCRay4 getRayPacket4(const int x, const int y) const;

    Vector3f m_TopLeft, m_TopRight, m_DownLeft;
    Vector3f m_FrontDir, m_RightDir, m_UpDir;
    Vector3f m_Position;
    Vector3f m_Rotation;
    Vector3f m_RotationMatrix;
    float m_Fov;
};

#endif // __CAMERA_H