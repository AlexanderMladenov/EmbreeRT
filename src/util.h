#ifndef __UTIL_H
#define __UTIL_H

#define GLM_FORCE_RADIANS 
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>

namespace embRT
{
    /*inline vec3 faceforward(const vec3& ray, const vec3& norm)
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
*/
    using namespace glm;
    inline mat4 CreateRotationMatrix(const vec3& rot)
    {
        return rotate(radians(rot.z), vec3(0, 0, 1)) *
               rotate(radians(rot.y), vec3(0, 1, 0)) *
               rotate(radians(rot.x), vec3(1, 0, 0));
    }
    inline float lenghtSqr(const vec3& vec)
    {
        return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
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