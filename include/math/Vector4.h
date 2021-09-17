//
// Vector4 class declaration
//

#ifndef VECTOR4_H
#define VECTOR4_H 1

#include <math/types.h>
#include <math/Vector3.h>

#include <cstring>
#include <xmmintrin.h>

#define VEC4_SIZE 0x10 // 4 * sizeof(F32) = 0x10 = 16 bytes

class Vector4 {
public:
    union {
        F32 _v[4];
        struct {
            F32 x, y, z, w;
        };
        __m128 _v4;
    };

    Vector4();
    Vector4(F32 X, F32 Y, F32 Z, F32 W = 1.0f);
    Vector4(const F32* values);
    Vector4(const Vector3& other); // homogeneous vector
    Vector4(const Vector4& other);

    Vector3 Point3D() const;
    Vector3 ToVector3() const;

    // binary operators
    Vector4& operator =(const Vector4& other);
    bool operator ==(const Vector4& other) const;
    Vector4& operator +=(const Vector4& other);
    Vector4& operator -=(const Vector4& other);
    Vector4& operator /=(const Vector4& other);
    Vector4& operator *=(const Vector4& other);
    Vector4& operator /=(const F32& other);
    Vector4& operator *=(const F32& factor);

    friend inline Vector4 operator +(Vector4 lhs, const Vector4& rhs) {
        return lhs += rhs;
    }
    friend inline Vector4 operator -(Vector4 lhs, const Vector4& rhs) {
        return lhs -= rhs;
    }
    friend inline Vector4 operator /(Vector4 lhs, const Vector4& rhs) {
        return lhs /= rhs;
    }
    friend inline Vector4 operator *(Vector4 lhs, const Vector4& rhs) {
        return lhs *= rhs;
    }

    Vector4& operator -();

    // access operators
    F32& operator [](const UI32 index);
    const F32& operator [](const UI32 index) const;

    const F32* ValuePtr() const;
    F32* ValuePtr();

    F32 Dot(const Vector4& other) const;
    F32 Length() const;
    Vector4 Normalize() const;
    F32 Sum() const;
};

// binary operators
Vector4 operator /(Vector4 lhs, const F32 rhs);
Vector4 operator *(Vector4 lhs, const F32 rhs);
Vector4 operator *(const F32 lhs, Vector4& rhs);

// stream operators
std::istream & operator >> (std::istream &inStream, Vector4&value);
std::ostream & operator << (std::ostream &outStream, const Vector4 &value);


#endif // ! VECTOR4_H