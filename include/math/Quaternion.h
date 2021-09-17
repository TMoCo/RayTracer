//
// Quaternion class declaration
//

#ifndef QUATERNION_H
#define QUATERNION_H 1

#include <math/Vector3.h>
#include <math/Vector4.h>

#include <cmath>

#define RADIANS(x) \
    x * M_PI / 180

class Quaternion {
private:
    Vector4 _q;

public:
    Quaternion(const Vector3& vector = {}, float scalar = 1.0f);
    Quaternion(float x, float y, float z, float w);
    Quaternion(const float* values);
    Quaternion(const Vector4& vector);

    // binary operators
    Quaternion &operator = (const Quaternion &other);
    bool operator ==(const Quaternion& other) const;
    Quaternion& operator +=(const Quaternion& other);
    Quaternion& operator -=(const Quaternion& other);
    Quaternion& operator /=(const Quaternion& other);
    Quaternion& operator *=(const Quaternion& other);
    Quaternion& operator /=(const float& other);
    Quaternion& operator *=(const float& factor);

    friend inline Quaternion operator +(Quaternion lhs, const Quaternion& rhs) {
        return lhs += rhs;
    }
    friend inline Quaternion operator -(Quaternion lhs, const Quaternion& rhs) {
        return lhs -= rhs;
    }
    friend inline Quaternion operator /(Quaternion lhs, const Quaternion& rhs) {
        return lhs /= rhs;
    }
    friend inline Quaternion operator *(Quaternion lhs, const Quaternion& rhs) {
        return lhs *= rhs;
    }

    // unary operators
    Quaternion& operator -();

    // access operators
    float& operator [](const uint32_t index);
    const float& operator [](const uint32_t index) const;

    // relative to a given quaternion
    Vector3 Axis() const;
    float Angle() const;

    float Norm() const;
    Quaternion Unit() const;
    Quaternion Conjugate() const;
    Quaternion Inverse() const;

    // quaternion builder
    static Quaternion AngleAxis(const Vector3& axis, float angle);
};

// binary operators
Quaternion operator /(Quaternion lhs, const float rhs);
Quaternion operator *(Quaternion lhs, const float rhs);
Quaternion operator *(const float lhs, Quaternion& rhs);

// stream operators
std::istream & operator >> (std::istream &inStream, Quaternion&value);
std::ostream & operator << (std::ostream &outStream, const Quaternion &value);

#endif // ! QUATERNION_H