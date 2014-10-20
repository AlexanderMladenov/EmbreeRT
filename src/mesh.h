#ifndef __TRIANGLE_MESH_H
#define __TRIANGLE_MESH_H

namespace embRT
{
    struct Mesh
    {
        PositionsNormalsUVsTris m_Data;
        
        std::vector<Triangle> extractIndexBuffer() const
        {
            auto trisCount = std::get<3>(m_Data).size();
            auto trisData = std::get<3>(m_Data);
            std::vector<Triangle> result;
            result.reserve(trisCount);

            for (auto i = 0; i < trisCount; i++)
            {
                Triangle T;
                T.v[0] = trisData[i].v[0];
                T.v[1] = trisData[i].v[1];
                T.v[2] = trisData[i].v[2];
                result.push_back(T);
            }
            return result;
        }
    };

} // namespace embRT

#endif