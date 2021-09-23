//
// Vector3 class declaration
//

#ifndef VECTOR3_H
#define VECTOR3_H 1

#include <core/types.h>

#include <iostream>
#include <xmmintrin.h>

class Vector3 {
public:
    union {
        F32 _v[4]; // still in fact 4 for simd operations (4th element is always ignored)
        __m128 _v4;
        struct {
            F32 x, y, z; 
        };
    };

    Vector3();
    Vector3(F32 X, F32 Y, F32 Z);
    Vector3(const F32* values);
    Vector3(const Vector3& other);

    // binary operators
    Vector3& operator =(const Vector3& other);
    bool operator ==(const Vector3& other) const;
    Vector3& operator +=(const Vector3& other);
    Vector3& operator -=(const Vector3& other);
    Vector3& operator /=(const Vector3& other);
    Vector3& operator *=(const Vector3& other);
    Vector3& operator /=(const F32& other);
    Vector3& operator *=(const F32& factor);

    friend inline Vector3 operator +(Vector3 lhs, const Vector3& rhs) {
        return lhs += rhs;
    }
    friend inline Vector3 operator -(Vector3 lhs, const Vector3& rhs) {
        return lhs -= rhs;
    }
    friend inline Vector3 operator /(Vector3 lhs, const Vector3& rhs) {
        return lhs /= rhs;
    }
    friend inline Vector3 operator *(Vector3 lhs, const Vector3& rhs) {
        return lhs *= rhs;
    }

    // unary operators
    Vector3 operator -() const;

    // access operators
    F32& operator [](const UI32 index);
    const F32& operator [](const UI32 index) const;
    
    F32 Dot(const Vector3& other) const;
    Vector3 Cross(const Vector3& other) const;
    F32 Length() const;
    Vector3 Normalize() const;
};

// binary operators
Vector3 operator /(Vector3 lhs, const F32& rhs);
Vector3 operator *(Vector3 lhs, const F32& rhs);
Vector3 operator *(const F32& lhs, Vector3 rhs);

// stream operators
std::istream & operator >> (std::istream &inStream, Vector3 &value);
std::ostream & operator << (std::ostream &outStream, const Vector3 &value);

#endif // ! VECTOR3_H