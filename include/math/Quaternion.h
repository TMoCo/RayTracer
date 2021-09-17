//
// Quaternion class declaration
//

#ifndef QUATERNION_H
#define QUATERNION_H 1

#include <math/Vector3.h>
#include <math/Vector4.h>
#include <math/types.h>

#include <cmath>

#define RADIANS(x) \
    x * M_PI / 180

class Quaternion {
private:
    union {
        Vector4 _q;
        struct {
            F32 vector[3];
            F32 scalar;
        };
    };

public:
    Quaternion(const Quaternion& other);
    Quaternion(const Vector3& vector = {}, F32 scalar = 1.0f);
    Quaternion(F32 x, F32 y, F32 z, F32 w);
    Quaternion(const F32* values);
    Quaternion(const Vector4& vector);

    // binary operators
    Quaternion &operator = (const Quaternion &other);
    bool operator ==(const Quaternion& other) const;
    Quaternion& operator +=(const Quaternion& other);
    Quaternion& operator -=(const Quaternion& other);
    Quaternion& operator /=(const Quaternion& other);
    Quaternion& operator *=(const Quaternion& other);
    Quaternion& operator /=(const F32& other);
    Quaternion& operator *=(const F32& factor);

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
    F32& operator [](const UI32 index);
    const F32& operator [](const UI32 index) const;

    // relative to a given quaternion
    Vector3 Axis() const;
    F32 Angle() const;

    F32 Norm() const;
    Quaternion Unit() const;
    Quaternion Conjugate() const;
    Quaternion Inverse() const;

    // quaternion builder
    static Quaternion AngleAxis(const Vector3& axis, F32 angle);
};

// binary operators
Quaternion operator /(Quaternion lhs, const F32 rhs);
Quaternion operator *(Quaternion lhs, const F32 rhs);
Quaternion operator *(const F32 lhs, Quaternion& rhs);

// stream operators
std::istream & operator >> (std::istream &inStream, Quaternion&value);
std::ostream & operator << (std::ostream &outStream, const Quaternion &value);

#endif // ! QUATERNION_H