#ifndef __TYPES_H
#define __TYPES_H

#ifdef _WIN32
#  define ALIGN(...) __declspec(align(__VA_ARGS__))
#else
#  define ALIGN(...) __attribute__((aligned(__VA_ARGS__)))
#endif

#include <vector>
#include <tuple>
#include <string>
#include <glm/vec3.hpp>
namespace embRT
{

    ALIGN(16) struct Vertex
    {
        float x, y, z;
    };

    struct Triangle
    {
        int v[3];
    };

    struct FullTriangleData
    {
        FullTriangleData()
        {
            memset(n, 0, 3 * sizeof(int));
        }
        int v[3];
        int n[3];
        int t[3];
    };

    typedef std::tuple<std::vector<Vertex>, std::vector<glm::vec3>, std::vector<float>, std::vector<FullTriangleData>> PositionsNormalsUVsTris;

} // namespace
#endif