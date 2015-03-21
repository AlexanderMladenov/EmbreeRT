#include "mesh.h"

namespace embRT
{
    Mesh::Mesh(const PositionsNormalsUVsTris& meshData) :
        meshDataOBJ(meshData),
        vertices(std::get<0>(meshDataOBJ)),
        triangles(std::get<3>(meshDataOBJ))
    {
        auto trisCount = triangles.size();
        indices.reserve(trisCount);

        for (auto i = 1; i < trisCount; i++)
        {
            Triangle T;
            T.v[0] = triangles[i].v[0];
            T.v[1] = triangles[i].v[1];
            T.v[2] = triangles[i].v[2];
            indices.emplace_back(T);
        }

    }

}