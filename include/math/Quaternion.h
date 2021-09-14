//
// Quaternion class declaration
//

#ifndef QUATERNION_H
#define QUATERNION_H 1

#include <math/Matrix4.h>
#include <math/Vector3.h>
#include <math/Vector4.h>

class Quaternion {
private:
    Vector3 _v;
    float _s;

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

    static Quaternion AngleAxis(const Vector3& axis, float angle);
    
    Vector3 Axis() const;
    float Angle() const;

    Matrix4 Rotation() const;

    float Norm() const;
    Quaternion Unit() const;
    Quaternion Conjugate() const;
    Quaternion Inverse() const;
};

// binary operators
Quaternion operator /(Quaternion lhs, const float rhs);
Quaternion operator *(Quaternion lhs, const float rhs);
Quaternion operator *(const float lhs, Quaternion& rhs);

// stream operators
std::istream & operator >> (std::istream &inStream, Quaternion&value);
std::ostream & operator << (std::ostream &outStream, const Quaternion &value);

#endif // ! QUATERNION_H