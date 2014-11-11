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

#ifndef __UTIL_H
#define __UTIL_H

namespace embRT
{
    std::mt19937 rng(std::time(NULL));

    inline vec3 faceforward(const vec3& ray, const vec3& norm)
    {
        if (dot(ray, norm) < 0) return norm;
        else return -norm;
    }

    inline SIMD::Vec3Packet faceforward(const SIMD::Vec3Packet& ray, const SIMD::Vec3Packet& norm)
    {
        __m128 dot = SIMD::Dot(ray, norm);
        __m128 ltZero = _mm_cmplt_ps(dot, SIMDConstants::zero);
        auto negNorm = -norm;
        return SIMD::Or(SIMD::And(ltZero, norm), SIMD::AndNot(ltZero, negNorm));
    }

    inline mat4 CreateRotationMatrix(const vec3& rot)
    {
        return rotate(radians(rot.z), vec3(0, 0, 1)) *
               rotate(radians(rot.y), vec3(0, 1, 0)) *
               rotate(radians(rot.x), vec3(1, 0, 0));
    }

    template <typename T = float>
    inline auto clamp(T x, T a, T b) -> T
    {
        return x <= a ? a : x >= b ? b : x;
    }

    typedef  std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;
    template <typename T = std::chrono::seconds>
    auto timePast(const TimePoint& start, const TimePoint& end) -> long long
    {
        return std::chrono::duration_cast<T>(end - start).count();
    }

} // namespace embRT

#endif