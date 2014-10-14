#ifndef __UTIL_H
#define __UTIL_H

namespace embRT
{
    mat4 CreateRotationMatrix(const vec3& rot)
    {
        return rotate(radians(rot.x), vec3(1, 0, 0)) *
            rotate(radians(rot.y), vec3(0, 1, 0)) *
            rotate(radians(rot.z), vec3(0, 0, 1));
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