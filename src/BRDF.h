//#ifndef __BRDF_H
//#define __BRDF_H
//
//namespace embRT
//{
//    //    vec3 hemisphereSample(const vec3& normal)
//    //    {
//    //        std::uniform_real_distribution<float> randoms;
//    //        auto u = randoms(rng);
//    //        auto v = randoms(rng);
//    //
//    //        auto theta = 2 * PI * u;
//    //        auto phi = acos(2 * v - 1) - PIHALF;
//    //
//    //        vec3 res(
//    //            cos(theta) * cos(phi),
//    //            sin(phi),
//    //            sin(theta) * cos(phi)
//    //            );
//    //
//    //        if (dot(res, normal) < 0)
//    //            res = -res;
//    //        return res;
//    //    }
//    //
//    class Lambert
//    {
//    public:
//        vec3 shade(const RTCScene& scene, RTCRay& ray, const vec3& xNormal) const
//        {
//
//            vec3 rayDir(ray.dir[0], ray.dir[1], ray.dir[2]);
//            vec3 rayStart(ray.org[0], ray.org[1], ray.org[2]);
//            // turn the normal vector towards us (if needed):
//            vec3 N = faceforward(rayDir, xNormal);
//
//            vec3 lightContrib(0.2);
//            vec3 intrPoint = rayStart + ray.tfar * rayDir;
//            vec3 shadowRayStart = intrPoint + 1e-3f * N;
//            //vec3 lightPos(-100, 1500, -5); // sponza pos
//            vec3 lightPos(100, 1000, -100);
//            vec3 shadowRayDir = normalize(lightPos - shadowRayStart);
//
//            RTCRay shadowRay;
//            shadowRay.org[0] = shadowRayStart.x;
//            shadowRay.org[1] = shadowRayStart.y;
//            shadowRay.org[2] = shadowRayStart.z;
//
//            shadowRay.dir[0] = shadowRayDir.x;
//            shadowRay.dir[1] = shadowRayDir.y;
//            shadowRay.dir[2] = shadowRayDir.z;
//            auto lightDist = (lightPos - shadowRayStart).length();
//
//            shadowRay.tfar = lightDist;
//            shadowRay.tnear = 0.f;
//            shadowRay.time = 0.f;
//            shadowRay.geomID = RTC_INVALID_GEOMETRY_ID;
//            shadowRay.primID = RTC_INVALID_GEOMETRY_ID;
//            shadowRay.mask = -1;
//            rtcOccluded(scene, shadowRay);
//            if (shadowRay.geomID == RTC_INVALID_GEOMETRY_ID)
//            {
//                vec3 lightDir = lightPos - intrPoint;
//                normalize(lightDir);
//
//                // get the Lambertian cosine of the angle between the geometry's normal and
//                // the direction to the light. This will scale the lighting:
//                float cosTheta = dot(lightDir, N);
//
//                lightContrib += vec3(1, 1, 1) * 800.f / lenghtSqr(intrPoint - lightPos) * cosTheta;
//            }
//            return vec3(0, 0.3, 1) * lightContrib;
//        }
//
//        vec3 shade(const RTCScene& scene, RTCRay& ray, const vec3& xNormal, const AreaLight& light) const
//        {
//            vec3 rayDir(ray.dir[0], ray.dir[1], ray.dir[2]);
//            vec3 rayStart(ray.org[0], ray.org[1], ray.org[2]);
//            // turn the normal vector towards us (if needed):
//            vec3 N = faceforward(rayDir, xNormal);
//
//            vec3 lightContrib(0.2);
//            vec3 intrPoint = rayStart + ray.tfar * rayDir;
//            vec3 shadowRayStart = intrPoint + 1e-3f * N;
//            static const auto numSamples = light.NumSamples();
//            vec3 avg(0);
//            for (auto i = 0; i < numSamples; i++)
//            {
//                vec3 lightPos;
//                vec3 lightCol;
//                light.illuminate(i, intrPoint, lightPos, lightCol);
//                vec3 shadowRayDir = normalize(lightPos - shadowRayStart);
//
//                RTCRay shadowRay;
//                shadowRay.org[0] = shadowRayStart.x;
//                shadowRay.org[1] = shadowRayStart.y;
//                shadowRay.org[2] = shadowRayStart.z;
//
//                shadowRay.dir[0] = shadowRayDir.x;
//                shadowRay.dir[1] = shadowRayDir.y;
//                shadowRay.dir[2] = shadowRayDir.z;
//
//                auto lightDist = (lightPos - shadowRayStart).length();
//
//                shadowRay.tfar = lightDist;
//                shadowRay.tnear = 0.f;
//                shadowRay.time = 0.f;
//                shadowRay.geomID = RTC_INVALID_GEOMETRY_ID;
//                shadowRay.primID = RTC_INVALID_GEOMETRY_ID;
//                shadowRay.mask = -1;
//                rtcOccluded(scene, shadowRay);
//                if (shadowRay.geomID == RTC_INVALID_GEOMETRY_ID)
//                {
//                    vec3 lightDir = lightPos - intrPoint;
//                    normalize(lightDir);
//
//                    // get the Lambertian cosine of the angle between the geometry's normal and
//                    // the direction to the light. This will scale the lighting:
//                    float cosTheta = dot(lightDir, N);
//                    if (cosTheta > 0)
//                        avg += lightCol / lenghtSqr(intrPoint - lightPos) * cosTheta;
//                }
//            }
//            lightContrib += avg / (float)numSamples;
//            return vec3(0, 0.3, 1) * lightContrib;
//        }
//    };
//
//
//}
//
//#endif