//
// Vector3 class declaration
//

#ifndef VECTOR3_H
#define VECTOR3_H 1

#include <iostream>
#include <cstdint>

#define VEC3_SIZE 0xc // 3 * sizeof(float) = 0xc = 12 bytes

typedef float vec3[3];

class Vector3 {
private:
    vec3 _v;

public:
    Vector3();
    Vector3(float X, float Y, float Z);
    Vector3(const float* values);
    Vector3(const Vector3& other);

    // binary operators
    Vector3& operator =(const Vector3& other);
    bool operator ==(const Vector3& other) const;
    Vector3& operator +=(const Vector3& other);
    Vector3& operator -=(const Vector3& other);
    Vector3& operator /=(const Vector3& other);
    Vector3& operator *=(const Vector3& other);
    Vector3& operator /=(const float& other);
    Vector3& operator *=(const float& factor);

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
    Vector3& operator -();

    // access operators
    float& operator [](const uint32_t index);
    const float& operator [](const uint32_t index) const;
    
    float Dot(const Vector3& other) const;
    Vector3 Cross(const Vector3& other) const;
    float Length() const;
    Vector3 Normalize() const;
};

// binary operators
Vector3 operator /(Vector3 lhs, const float rhs);
Vector3 operator *(Vector3 lhs, const float rhs);
Vector3 operator *(const float lhs, Vector3& rhs);

// stream operators
std::istream & operator >> (std::istream &inStream, Vector3 &value);
std::ostream & operator << (std::ostream &outStream, const Vector3 &value);

#endif // ! VECTOR3_H