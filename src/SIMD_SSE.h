#ifndef __SIMD__SSE
#define __SIMD__SSE

namespace embRT
{
    namespace SIMDConstants
    {
        const __m128 minusOne = _mm_set1_ps(-1.0f);
        const __m128 zero = _mm_set1_ps(0.0f);
        const __m128 oneThird = _mm_set1_ps(1.0f / 3.0f);
        const __m128 oneHalf = _mm_set1_ps(1.0f / 2.0f);
        const __m128 one = _mm_set1_ps(1.0f);
        const __m128 two = _mm_set1_ps(2.0f);
        const __m128 three = _mm_set1_ps(3.0f);
        const __m128 sixty = _mm_set1_ps(60.0f);

    }

    using namespace glm;

    namespace SIMD
    {
        struct Matrix4
        {

            union
            {
                float m[4][4];
                __m128 rows[4];
            };

            void Set(const vec4& row0, const vec4& row1, const vec4& row2, const vec4& row3)
            {
                rows[0] = _mm_set_ps(row0.w, row0.z, row0.y, row0.x);
                rows[1] = _mm_set_ps(row1.w, row1.z, row1.y, row1.x);
                rows[2] = _mm_set_ps(row2.w, row2.z, row2.y, row2.x);
                rows[3] = _mm_set_ps(row3.w, row3.z, row3.y, row3.x);
            }

            void Set(const mat4& m)
            {
                Set(m[0], m[1], m[2], m[3]);
            }

            vec4 Extract(size_t row) const
            {
                return vec4(m[row][0], m[row][1], m[row][2], m[row][3]);
            }

            Matrix4 operator*(const Matrix4& b)
            {
                _mm_prefetch((char*)&(m[0][0]), _MM_HINT_T0);
                _mm_prefetch((char*)&(b.m[0][0]), _MM_HINT_T0);

                Matrix4 result;
                Matrix4 bTransp = b;

                _MM_TRANSPOSE4_PS(bTransp.rows[0], bTransp.rows[1], bTransp.rows[2], bTransp.rows[3]);

                __m128 r8 = _mm_mul_ps(rows[0], bTransp.rows[0]);
                __m128 r9 = _mm_mul_ps(rows[0], bTransp.rows[1]);
                __m128 r10 = _mm_mul_ps(rows[0], bTransp.rows[2]);
                __m128 r11 = _mm_mul_ps(rows[0], bTransp.rows[3]);
                r8 = _mm_hadd_ps(r8, r9);
                r9 = _mm_hadd_ps(r10, r11);
                result.rows[0] = _mm_hadd_ps(r8, r9);

                r8 = _mm_mul_ps(rows[1], bTransp.rows[0]);
                r9 = _mm_mul_ps(rows[1], bTransp.rows[1]);
                r10 = _mm_mul_ps(rows[1], bTransp.rows[2]);
                r11 = _mm_mul_ps(rows[1], bTransp.rows[3]);
                r8 = _mm_hadd_ps(r8, r9);
                r9 = _mm_hadd_ps(r10, r11);
                result.rows[1] = _mm_hadd_ps(r8, r9);

                r8 = _mm_mul_ps(rows[2], bTransp.rows[0]);
                r9 = _mm_mul_ps(rows[2], bTransp.rows[1]);
                r10 = _mm_mul_ps(rows[2], bTransp.rows[2]);
                r11 = _mm_mul_ps(rows[2], bTransp.rows[3]);
                r8 = _mm_hadd_ps(r8, r9);
                r9 = _mm_hadd_ps(r10, r11);
                result.rows[2] = _mm_hadd_ps(r8, r9);

                r8 = _mm_mul_ps(rows[3], bTransp.rows[0]);
                r9 = _mm_mul_ps(rows[3], bTransp.rows[1]);
                r10 = _mm_mul_ps(rows[3], bTransp.rows[2]);
                r11 = _mm_mul_ps(rows[3], bTransp.rows[3]);
                r8 = _mm_hadd_ps(r8, r9);
                r9 = _mm_hadd_ps(r10, r11);
                result.rows[3] = _mm_hadd_ps(r8, r9);

                return result;
            }

        };

        struct Vec3Packet
        {

            union
            {
                __m128 x; struct { float x0; float x1; float x2; float x3; };
            };

            union
            {
                __m128 y; struct { float y0; float y1; float y2; float y3; };
            };

            union
            {
                __m128 z; struct { float z0; float z1; float z2; float z3; };
            };

            void Set(const vec3& v)
            {
                x = _mm_set1_ps(v.x);
                y = _mm_set1_ps(v.y);
                z = _mm_set1_ps(v.z);
            }

            void Set(const vec3& a, const vec3& b, const vec3& c, const vec3& d)
            {
                x = _mm_set_ps(a.x, b.x, c.x, d.x);
                y = _mm_set_ps(a.y, b.y, c.y, d.y);
                z = _mm_set_ps(a.z, b.z, c.z, d.z);
            }

            vec3 Extract(size_t index)
            {
                switch (index)
                {
                case 0: return vec3(x0, y0, z0);
                case 1: return vec3(x1, y1, z1);
                case 2: return vec3(x2, y2, z2);
                case 3: return vec3(x3, y3, z3);
                }

                return vec3(0.f);
            }

        };

        inline Vec3Packet operator+(const Vec3Packet& a, const Vec3Packet& b)
        {
            Vec3Packet result;
            result.x = _mm_add_ps(a.x, b.x);
            result.y = _mm_add_ps(a.y, b.y);
            result.z = _mm_add_ps(a.z, b.z);
            return result;
        }

        inline Vec3Packet operator-(const Vec3Packet& a, const Vec3Packet& b)
        {
            Vec3Packet result;
            result.x = _mm_sub_ps(a.x, b.x);
            result.y = _mm_sub_ps(a.y, b.y);
            result.z = _mm_sub_ps(a.z, b.z);
            return result;
        }

        inline Vec3Packet operator*(const Vec3Packet& a, __m128 scalar)
        {
            Vec3Packet result;
            result.x = _mm_mul_ps(a.x, scalar);
            result.y = _mm_mul_ps(a.y, scalar);
            result.z = _mm_mul_ps(a.z, scalar);
            return result;
        }

        inline Vec3Packet operator/(const Vec3Packet& a, __m128 scalar)
        {
            Vec3Packet result;
            result.x = _mm_div_ps(a.x, scalar);
            result.y = _mm_div_ps(a.y, scalar);
            result.z = _mm_div_ps(a.z, scalar);
            return result;
        }

        inline __m128 Dot(const Vec3Packet& a, const Vec3Packet& b)
        {
            auto x2 = _mm_mul_ps(a.x, b.x);
            auto y2 = _mm_mul_ps(a.y, b.y);
            auto z2 = _mm_mul_ps(a.z, b.z);
            return _mm_add_ps(_mm_add_ps(x2, y2), z2);
        }

        inline __m128 Length2(const Vec3Packet& a)
        {
            auto temp = _mm_mul_ps(a.x, a.x);
            auto y2 = _mm_mul_ps(a.y, a.y);
            auto z2 = _mm_mul_ps(a.z, a.z);

            temp = _mm_add_ps(temp, y2);
            temp = _mm_add_ps(temp, z2);
            return temp;
        }

        inline void Normalize(Vec3Packet& a)
        {
            auto x2 = _mm_mul_ps(a.x, a.x);
            auto y2 = _mm_mul_ps(a.y, a.y);
            auto z2 = _mm_mul_ps(a.z, a.z);

            x2 = _mm_add_ps(x2, y2);
            x2 = _mm_add_ps(x2, z2);

            __m128 nr = _mm_rsqrt_ps(x2);
            __m128 muls = _mm_mul_ps(_mm_mul_ps(x2, nr), nr);
            x2 = _mm_mul_ps(_mm_mul_ps(SIMDConstants::oneHalf, nr), _mm_sub_ps(SIMDConstants::three, muls));

            a.x = _mm_mul_ps(a.x, x2);
            a.y = _mm_mul_ps(a.y, x2);
            a.z = _mm_mul_ps(a.z, x2);
        }

        // TODO implementation
        /* inline Vec3Packet operator*(const Matrix4& mat, const Vec3Packet& vec)
         {
         _mm_prefetch((char*)&(mat.m[0][0]), _MM_HINT_T0);
         Matrix4 matTransp = mat;
         _MM_TRANSPOSE4_PS(matTransp.rows[0], matTransp.rows[1], matTransp.rows[2], matTransp.rows[3]);
         Vec3Packet result;

         auto x = Dot(vec, vec);


         }*/

        inline Vec3Packet And(const Vec3Packet& a, const Vec3Packet& b)
        {
            Vec3Packet result;
            result.x = _mm_and_ps(a.x, b.x);
            result.y = _mm_and_ps(a.y, b.y);
            result.z = _mm_and_ps(a.z, b.z);
            return result;
        }

        inline Vec3Packet And(__m128 a, const Vec3Packet& b)
        {
            Vec3Packet result;
            result.x = _mm_and_ps(a, b.x);
            result.y = _mm_and_ps(a, b.y);
            result.z = _mm_and_ps(a, b.z);
            return result;
        }

        inline Vec3Packet AndNot(const Vec3Packet& a, const Vec3Packet& b)
        {
            Vec3Packet result;
            result.x = _mm_andnot_ps(a.x, b.x);
            result.y = _mm_andnot_ps(a.y, b.y);
            result.z = _mm_andnot_ps(a.z, b.z);
            return result;
        }

        inline Vec3Packet AndNot(__m128 a, const Vec3Packet& b)
        {
            Vec3Packet result;
            result.x = _mm_andnot_ps(a, b.x);
            result.y = _mm_andnot_ps(a, b.y);
            result.z = _mm_andnot_ps(a, b.z);
            return result;
        }

        inline Vec3Packet Or(const Vec3Packet& a, const Vec3Packet& b)
        {
            Vec3Packet result;
            result.x = _mm_or_ps(a.x, b.x);
            result.y = _mm_or_ps(a.y, b.y);
            result.z = _mm_or_ps(a.z, b.z);
            return result;
        }

        inline Vec3Packet Or(__m128 a, const Vec3Packet& b)
        {
            Vec3Packet result;
            result.x = _mm_or_ps(a, b.x);
            result.y = _mm_or_ps(a, b.y);
            result.z = _mm_or_ps(a, b.z);
            return result;
        }
    } // namespace SIMD
} // namespace embRT
#endif
