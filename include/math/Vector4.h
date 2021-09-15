//
// Vector4 class declaration
//

#ifndef VECTOR4_H
#define VECTOR4_H 1

#include <math/Vector3.h>

#include <cstring>
#include <xmmintrin.h>

#define VEC4_SIZE 0x10 // 4 * sizeof(float) = 0x10 = 16 bytes

typedef union vec4 {
    float v[4];
    struct { float x, y, z, w; };
    __m128 v4;

    inline vec4() { std::memset(this, 0, VEC4_SIZE); }
    inline vec4(const vec4& other)   { std::memcpy(this, &other, VEC4_SIZE); }
    inline vec4(const __m128& other) { std::memcpy(this, &other, VEC4_SIZE); }

    inline vec4& operator =(const __m128& vector) { v4 = vector; return *this; };
    inline vec4& operator =(const float* vector) { std::memcpy(v, vector, VEC4_SIZE); return *this; };

    inline float& operator [](const uint32_t index) { return v[index]; }
    inline const float& operator [](const uint32_t index) const { return v[index]; }
} vec4;


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