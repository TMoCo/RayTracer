//
// Quaternion class definition
//

#include <core/core.h>

#include <math/Quaternion.h>

#include <cmath>
#include <cstring>

Quaternion::Quaternion(const Quaternion& other) : _q{other._q} {}

Quaternion::Quaternion(const Vector3& v, float s) : vector{}, scalar(s) {
    std::memcpy(vector, v._v, SIZEOF_VEC3);
    scalar = s;
}

Quaternion::Quaternion(float x, float y, float z, float w) : _q{x, y, z, w} {} // avoid

Quaternion::Quaternion(const float* values) : _q{values} {}

Quaternion::Quaternion(const Vector4& vector) : _q{vector} {}

// assignment operator
Quaternion& Quaternion::operator = (const Quaternion &other) {
    std::memcpy(this, &other, SIZEOF_VEC4);
    return *this;
}

bool Quaternion::operator ==(const Quaternion& other) const {
    return std::memcmp(this, &other, SIZEOF_VEC4) == 0; 
}

Quaternion& Quaternion::operator +=(const Quaternion& other) {
    _q += other._q;
    return *this;
}

Quaternion& Quaternion::operator -=(const Quaternion& other) {
    _q -= other._q;
    return *this;
}

Quaternion& Quaternion::operator /=(const Quaternion& other) {
    _q *= other.Inverse()._q;
    return *this;
}

Quaternion& Quaternion::operator *=(const Quaternion& other) {
    Vector3 v1{&_q[0]}, v2{&other._q[0]};
    float s1 = _q[3], s2 = other._q[3];
    *this = Quaternion{s1 * v2 + s2 * v1 + v1.Cross(v2), s1 * s2 - v1.Dot(v2)}; // must be a better way
    return *this;
}

Quaternion& Quaternion::operator /=(const float& factor) {
    _q /= factor;
    return *this;
}

Quaternion& Quaternion::operator *=(const float& factor) {
    _q *= factor;
    return *this;
}

Quaternion& Quaternion::operator -() {
    return *this *= -1.0f;
}

float& Quaternion::operator [](const uint32_t index) {
    return _q[index];
}

const float& Quaternion::operator [](const uint32_t index) const {
    return _q[index];
}

Quaternion Quaternion::AngleAxis(const Vector3& axis, float angle) {
    return Quaternion(axis.Normalize() * std::sin(angle * 0.5f), 
        std::cos(angle * 0.5f)); 
}

Quaternion Quaternion::Rotation(const Vector3& from, const Vector3& to) {
    return {from.Cross(to), std::sqrt(from.Dot(from) * to.Dot(to) + from.Dot(to))};
}

Vector3 Quaternion::RotateVector(const Vector3& vector, const Quaternion& quaternion) {
    Vector3 qVec = quaternion.vector;
    Vector3 t = (2.0f * qVec).Cross(vector);
    return vector + quaternion.scalar * t + qVec.Cross(t);
}

Vector3 Quaternion::Vector() const {
    return vector;
}

float Quaternion::Scalar() const {
    return scalar;
}

float Quaternion::Norm() const {
    return std::sqrt(_q.Dot(_q));
}

Quaternion Quaternion::Unit() const {
    return *this / Norm(); 
}

Quaternion Quaternion::Conjugate() const {
    return {-Vector3{vector}, scalar};
}

Quaternion Quaternion::Inverse() const {
    return Conjugate() / _q.Dot(_q);
}

Quaternion operator /(Quaternion lhs, const float rhs) {
    return lhs /= rhs;
}

Quaternion operator *(const float lhs, Quaternion& rhs) {
    return rhs *= lhs;
}

Quaternion operator *(Quaternion lhs, const float rhs) {
    return rhs * lhs;
}

std::istream& operator >> (std::istream &inStream, Quaternion &quaternion) {
    return inStream >> quaternion._q; // Vector4
}

std::ostream & operator << (std::ostream &outStream, const Quaternion &quaternion) {
    return outStream << quaternion._q; 
}
