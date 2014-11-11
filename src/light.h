#ifndef __LIGHT_H
#define __LIGHT_H

namespace embRT
{
    struct AreaLight
    {
        ModelTransform transform;
        int xSubd, ySubd;
        float area;
        vec3 center;
        vec3 color;
        float power;

        AreaLight(const vec3&col, int xs = 4, int ys = 4, float power = 14): xSubd(xs), ySubd(ys), color(col), power(power)
        {
            center = transform.point(vec3(0));
            vec3 a = transform.point(vec3(-0.5, 0.0, -0.5));
            vec3 b = transform.point(vec3(0.5, 0.0, -0.5));
            vec3 c = transform.point(vec3(0.5, 0.0, 0.5));
            float width = (float) (b - a).length();
            float height = (float) (b - c).length();
            area = width * height;
        }

        const int NumSamples() const
        {
            return xSubd * ySubd;
        }

        void illuminate(int sampleIdx, const vec3& shadePos, vec3& samplePos, vec3& color)
        {

            std::uniform_real_distribution<float> roll;
            // convert the shade point onto the light's canonic space:
            vec3 shadePosCanonical = transform.undoPoint(shadePos);

            // shade point "behind" the lamp?
            if (shadePosCanonical.y > 0) 
            {
                color = vec3(0);
                return;
            }

            float sx = (sampleIdx % xSubd + roll(rng)) / xSubd;
            float sy = (sampleIdx / xSubd + roll(rng)) / ySubd;

            vec3 sampleCanonical(sx - 0.5, 0, sy - 0.5);
            samplePos = transform.point(sampleCanonical);
            vec3 shadePos_LS = shadePosCanonical - sampleCanonical;
            // return light color, attenuated by the angle of incidence
            // (the cosine between the light's direction and the normed ray toward the hitpos)
            color = this->color * (area * power * float(dot(vec3(0, -1, 0), shadePos_LS) / shadePos_LS.length()));
        }

    };
}

#endif