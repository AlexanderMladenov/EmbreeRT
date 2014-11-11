#ifndef __BRDF_H
#define __BRDF_H

namespace embRT
{
    vec3 hemisphereSample(const vec3& normal)
    {
        std::uniform_real_distribution<float> randoms;
        auto u = randoms(RandomGen);
        auto v = randoms(RandomGen);

        auto theta = 2 * PI * u;
        auto phi = acos(2 * v - 1) - PIHALF;

        vec3 res(
            cos(theta) * cos(phi),
            sin(phi),
            sin(theta) * cos(phi)
            );

        if (dot(res, normal) < 0)
            res = -res;
        return res;
    }
    
    class Lambert
    {
        public:
        vec3 shade(const RTCRay& ray, const vec3& xNormal, const AreaLight& light)
        {
            vec3 rayDir(ray.dir[0], ray.dir[1], ray.dir[2]);
            // turn the normal vector towards us (if needed):
            vec3 N = faceforward(rayDir, xNormal);

            auto lightContrib = vec3(0.01);

            //for (auto i = 0 i < (int) scene.lights.size(); i++) {
            const static int numSamples = light.NumSamples();
                vec3 avgColor(0);
                for (auto j = 0; j < numSamples; j++) 
                {
                    vec3 lightPos;
                    vec3 lightColor;
                    light.illuminate(j, data.p, lightPos, lightColor);
                    if (lightColor.intensity() != 0 && testVisibility(data.p + N * 1e-6, lightPos)) {
                        Vector lightDir = lightPos - data.p;
                        lightDir.normalize();

                        // get the Lambertian cosine of the angle between the geometry's normal and
                        // the direction to the light. This will scale the lighting:
                        double cosTheta = dot(lightDir, N);
                        if (cosTheta > 0)
                            avgColor += lightColor / (data.p - lightPos).lengthSqr() * cosTheta;
                    }
                }
                lightContrib += avgColor / numSamples;
            //}
            return diffuseColor * lightContrib;
        }

        vec3 diffuseColor = vec(0, 0.3, 1);
    };

    class LambertBRDF
    {
        public:
        vec3 eval(const RTCRay& x, const RTCRay& w_in, const vec3& w_out)
        {
            vec3 w_in_dir(w_in.dir[0], w_in.dir[1], w_in.dir[2]);
            //vec3 x_normal(x.n)
           /* vec3 N = faceforward(w_in_dir, x.normal);
            Color diffuseColor = this->color;
            return diffuseColor * (1 / PI) * max(0.0, dot(w_out.dir, N));*/
            return m_Color;
        }

        void spawnRay(const vec3& xNormal, const RTCRay& w_in, RTCRay& w_out, vec3& colorEval, float& pdf)
        {
            vec3 w_in_dir(w_in.dir[0], w_in.dir[1], w_in.dir[2]);
            vec3 N = faceforward(w_in_dir, xNormal);

            w_out = w_in;
            vec3 w_out_org(w_out.org[0], w_out.org[1], w_out.org[2]);
            w_out_org += 1e-4f * N;
            w_out.org[0] = w_out_org.x;
            w_out.org[1] = w_out_org.y;
            w_out.org[2] = w_out_org.z;

            vec3 w_out_dir = hemisphereSample(N);
            normalize(w_out_dir);
            w_out.dir[0] = w_out_dir.x;
            w_out.dir[1] = w_out_dir.y;
            w_out.dir[2] = w_out_dir.z;

            colorEval = vec3(0.1) +  m_Color * (1 / PI) * std::max(0.f, dot(w_out_dir, N));
            pdf = 1.f / (2.f * PI);
        }

        vec3 m_Color = vec3(1, 0.3, 0);
    };

}

#endif