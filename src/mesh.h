#ifndef __TRIANGLE_MESH_H
#define __TRIANGLE_MESH_H

#include <vector>
#include <tuple>

#include "types.h"

namespace embRT
{
    struct Mesh
    {
        std::string name; // name of mesh file
        PositionsNormalsUVsTris meshDataOBJ; // mesh data as read from obj file
        std::vector<Triangle> indices; // index buffer for direct use with embree
        std::vector<Vertex>& vertices; // vertex buffer for direct use with embree

        // array with vertex indices, normal indices and texture indices (for meshDataOBJ) for any other use
        std::vector<FullTriangleData>& triangles; 


        Mesh(const PositionsNormalsUVsTris& meshData);

        inline std::vector<Vertex> GenerateVertexBufferAligned() const
        {
            return std::get<0>(meshDataOBJ);
        }

        std::vector<Triangle> GenerateIndexBufferAligned() const;
    };

} // namespace embRT

#endif