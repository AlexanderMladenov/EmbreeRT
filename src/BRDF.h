//#ifndef __BRDF_H
//#define __BRDF_H
//
//namespace embRT
//{
//    SIMD::Vec3Packet& hemisphereSample(const SIMD::Vec3Packet& normals)
//    {
//        std::uniform_real_distribution<float> randoms;
//        __m128 u = _mm_set_ps(randoms(RandomGen), randoms(RandomGen), randoms(RandomGen), randoms(RandomGen));
//        __m128 v = _mm_set_ps(randoms(RandomGen), randoms(RandomGen), randoms(RandomGen), randoms(RandomGen));
//
//        __m128 theta = _mm_mul_ps(u, SIMDConstants::twoPi);
//        __m128 a =  _mm_sqrt_ps(v);
//        float h[4];
//        memcpy(h, &a, sizeof(__m128));
//        __m128 b = _mm_set_ps(acosf(h[0]), acosf(h[1]), acosf(h[2]), acosf(h[3]));
//        __m128 phi = _mm_sub_ps(b, SIMDConstants::PiOverTwo);
//
//        vec3 res(
//            cos(theta) * cos(phi),
//            sin(phi),
//            sin(theta) * cos(phi)
//            );
//
//        if (dot(res, normal) < 0)
//            res = -res;
//        return res;
//    }
//
//    struct Lambert
//    {
//        /*
//        std::array<vec3, 4> Evaluate4(const RTCRay4& w_in, const RTCRay4& w_out)
//        {
//            vec3 N = faceforward(w_in.dir, x.normal);
//            vec3 diffuseColor = m_Color;
//            return diffuseColor * (1.f / (float)PI) * std::max(0.f, dot(w_out.dir, N));
//        }*/
//
//        void Lambert::spawnRay(const SIMD::Vec3Packet& xNormal, const RTCRay4& w_in, RTCRay4& w_out, std::array<vec3, 4>& colorEval, float& pdf)
//        {
//            SIMD::Vec3Packet dirPack;
//            memcpy(&dirPack.x, w_in.dirx, sizeof(__m128));
//            memcpy(&dirPack.y, w_in.diry, sizeof(__m128));
//            memcpy(&dirPack.z, w_in.dirz, sizeof(__m128));
//            //auto N = faceforward(dirPack, xNormal);
//            SIMD::Vec3Packet diffuseColor;
//            diffuseColor.Set(m_Color);
//
//            /*
//            Color diffuseColor = this->color;
//            if (texture) diffuseColor = texture->getTexColor(w_in, x.u, x.v, N);
//
//            w_out = w_in;
//
//            w_out.depth++;
//            w_out.start = x.p + N * 1e-6;
//            w_out.dir = hemisphereSample(N);
//            w_out.flags = w_out.flags | RF_DIFFUSE;
//            colorEval = diffuseColor * (1 / PI) * max(0.0, dot(w_out.dir, N));
//            pdf = dot(w_out.dir, x.normal) / (PI);*/
//        }
//
//        vec3 m_Color = vec3(1, 0.3, 0);
//    };
//
//}
//
//#endif