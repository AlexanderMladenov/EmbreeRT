#ifndef __TRANSFORMATION_H
#define __TRANSFORMATION_H

namespace embRT
{
    class ModelTransform
    {
        public:
        ModelTransform():
            m_Transform(mat4(1.f)),
            m_InverseTransform(inverse(m_Transform)),
            m_NormalTransform(mat4(1.f)) {}

        void reset()
        {
            m_Transform = mat4(1);
            m_InverseTransform = inverse(m_Transform);
            m_NormalTransform = mat4(1);
        }

        void scale(const vec3& sc)
        {
            m_Transform *= glm::scale(sc);
            m_InverseTransform = inverse(m_Transform);
            m_NormalTransform = transpose(m_InverseTransform);
        }

        void rotate(const vec3& rotation)
        {
            m_Transform *= CreateRotationMatrix(rotation);
            m_InverseTransform = inverse(m_Transform);
            m_NormalTransform = transpose(m_InverseTransform);
        }

        void translate(const vec3& translation)
        {
            m_Transform *= glm::translate(translation);
            m_InverseTransform = inverse(m_Transform);
            m_NormalTransform = transpose(m_InverseTransform);
        }

        vec3 point(const vec3& p) const
        {
            return vec3(m_Transform * vec4(p, 1));
        }

        vec3 undoPoint(const vec3& p) const
        {
            return vec3(m_InverseTransform * vec4(p, 1));
        }

        vec3 direction(const vec3& d) const
        {
            return vec3(m_Transform * vec4(d, 0));
        }
        vec3 undoDirection(const vec3& d) const
        {
            return vec3(m_InverseTransform * vec4(d, 0));
        }

        vec3 normal(const vec3& d) const
        {
            return vec3(m_NormalTransform * vec4(d, 0));
        }

        RTCRay ray(const RTCRay& inputRay) const
        {
            RTCRay result = inputRay;
            vec3 inputStart(inputRay.org[0], inputRay.org[1], inputRay.org[2]);
            vec3 inputDir(inputRay.dir[0], inputRay.dir[1], inputRay.dir[2]);
            auto transformStart = point(inputStart);
            auto transformDir = direction(inputDir);

            result.org[0] = transformStart.x;
            result.org[1] = transformStart.y;
            result.org[2] = transformStart.z;

            result.dir[0] = transformDir.x;
            result.dir[1] = transformDir.y;
            result.dir[2] = transformDir.z;

            return result;
        }

        RTCRay undoRay(const RTCRay& inputRay) const
        {
            RTCRay result = inputRay;
            vec3 inputStart(inputRay.org[0], inputRay.org[1], inputRay.org[2]);
            vec3 inputDir(inputRay.dir[0], inputRay.dir[1], inputRay.dir[2]);

            auto transformStart = undoPoint(inputStart);
            auto transformDir = undoDirection(inputDir);

            result.org[0] = transformStart.x;
            result.org[1] = transformStart.y;
            result.org[2] = transformStart.z;

            result.dir[0] = transformDir.x;
            result.dir[1] = transformDir.y;
            result.dir[2] = transformDir.z;

            return result;
        }

        private:
        mat4 m_Transform;
        mat4 m_InverseTransform;
        mat4 m_NormalTransform;
    };
}
#endif