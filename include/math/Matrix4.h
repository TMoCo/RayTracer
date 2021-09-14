//
// Matrix4 class declaration
//

#ifndef MATRIX4_H
#define MATRIX4_H 1

#include <math/Vector4.h>

#define MAT4_ELEMENTS 0x10 // 0x10 = 16 elements 
#define MAT4_SIZE 0x40 // 4 * sizeof(vec4) = 0x40 = 64 bytes

typedef float mat4[16];

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

    Matrix4 Transpose();
    static void Transpose(Matrix4& mat);

    Matrix4 Translate(const Vector3& v);
    static void Translate(Matrix4& mat, const Vector3& v);

    static Matrix4 Identity();
};

// binary operators
Matrix4 operator *(Matrix4 lhs, const float rhs);
Matrix4 operator *(const float lhs, Matrix4& rhs);
Matrix4 operator /(Matrix4 lhs, const float rhs);

// stream i/o
std::istream & operator >> (std::istream &outStream, Matrix4 &matrix);
std::ostream & operator << (std::ostream &outStream, const Matrix4 &matrix);

#endif // !MATRIX4_H