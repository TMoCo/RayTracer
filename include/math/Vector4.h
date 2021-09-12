//
// Vector4 class declaration
//

#ifndef VECTOR4_H
#define VECTOR4_H 1

#include <math/Vector3.h>

#define VEC4_SIZE 0x10 // 4 * sizeof(float) = 0x10 = 16 bytes

typedef float vec4[4];

class Vector4 {
public:
    vec4 _v;

    Vector4();
    Vector4(float X, float Y, float Z, float W = 1.0f);
    Vector4(const float* values);
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
    Vector4& operator /=(const float& other);
    Vector4& operator *=(const float& factor);

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
    float& operator [](const uint32_t index);
    const float& operator [](const uint32_t index) const;

    const float* ValuePtr() const;
    float* ValuePtr();

    float Dot(const Vector4& other) const;
    float Length() const;
    Vector4 Normalize() const;
    float Sum() const;
};

// binary operators
Vector4 operator /(Vector4 lhs, const float rhs);
Vector4 operator *(Vector4 lhs, const float rhs);
Vector4 operator *(const float lhs, Vector4& rhs);

// stream operators
std::istream & operator >> (std::istream &inStream, Vector4&value);
std::ostream & operator << (std::ostream &outStream, const Vector4 &value);


#endif // ! VECTOR4_H