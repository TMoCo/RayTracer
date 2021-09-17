//
// Matrix4 class declaration
//

#ifndef MATRIX4_H
#define MATRIX4_H 1

#include <math/Vector4.h>
#include <math/Quaternion.h>

#define MAT4_ELEMENTS 0x10 // 0x10 = 16 elements 
#define MAT4_SIZE 0x40 // 4 * sizeof(vec4) = 0x40 = 64 bytes

typedef union mat4 {
    float m[16];
    __m128 m16[4];

    inline mat4() { std::memset(this, 0, MAT4_SIZE); } // make sure initialised to 0
    inline mat4(const mat4& other)   { std::memcpy(this, &other, MAT4_SIZE); }
    inline mat4(const __m128* other) { std::memcpy(this, other, MAT4_SIZE); }

    inline float& operator [](const uint32_t index) { return m[index]; }
    inline const float& operator [](const uint32_t index) const { return m[index]; }

} mat4;

// typedef float mat4[16];

class Matrix4 {
private:
    mat4 _m; // store matrix in column major format

public:
    Matrix4();
    Matrix4(const Matrix4& other);
    Matrix4(const float* values);

    Matrix4& operator =(const Matrix4& other);
    bool operator ==(const Matrix4& other) const;
    Matrix4& operator +=(const Matrix4& other);
    Matrix4& operator -=(const Matrix4& other);
    Matrix4& operator *=(const Matrix4& other);
    Matrix4& operator /=(const float& other);
    Matrix4& operator *=(const float& factor);

    friend inline Matrix4 operator *(Matrix4 lhs, const Matrix4& rhs) {
        return lhs *= rhs;
    }
    friend inline Matrix4 operator +(Matrix4 lhs, const Matrix4& rhs) {
        return lhs += rhs;
    }
    friend inline Matrix4 operator -(Matrix4 lhs, const Matrix4& rhs) {
        return lhs -= rhs;
    }

    // access operators
    float* operator [](const uint32_t index);
    const float* operator [](const uint32_t index) const;

    // static matrix builders
    static Matrix4 Identity();
    static Matrix4 Transpose(const Matrix4& mat);
    static Matrix4 TranslationMatrix(const Vector3& v);
    static Matrix4 RotationMatrix(const Quaternion& q);
    static Matrix4 ScaleMatrix(const float& s);
    static Matrix4 Perspective(float FOV, float aspectRatio, float near, float far);
};

// binary operators
Matrix4 operator *(Matrix4 lhs, const float rhs);
Matrix4 operator *(const float lhs, Matrix4& rhs);
Matrix4 operator /(Matrix4 lhs, const float rhs);

// stream i/o
std::istream & operator >> (std::istream &outStream, Matrix4 &matrix);
std::ostream & operator << (std::ostream &outStream, const Matrix4 &matrix);

#endif // !MATRIX4_H