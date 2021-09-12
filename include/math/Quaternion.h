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
    Vector4 _q;

public:
    Quaternion();
    Quaternion(float x, float y, float z, float w);
    Quaternion(float scalar);
    Quaternion(const Vector3& vector);
    Quaternion(const Vector3& vector, float scalar);
    Quaternion(const Vector4& vector);

    // assignment operator
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

    Quaternion& operator -();

    // access operators
    float& operator [](const uint32_t index);
    const float& operator [](const uint32_t index) const;
    Vector4* Vec4Ptr();
    const Vector4* Vec4Ptr() const;

    static Quaternion AngleAxis(const Vector3& axis, float angle);
    
    Vector3 Axis() const;
    float Angle() const;

    float Norm() const;
    Quaternion Unit() const;
    Quaternion Conjugate() const;
    Quaternion Inverse() const;
};

// binary operators
Vector4 operator /(Vector4 lhs, const float rhs);
Vector4 operator *(Vector4 lhs, const float rhs);
Vector4 operator *(const float lhs, Vector4& rhs);

// stream operators
std::istream & operator >> (std::istream &inStream, Vector4&value);
std::ostream & operator << (std::ostream &outStream, const Vector4 &value);

#endif // ! QUATERNION_H