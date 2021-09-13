//
// Quaternion class definition
//

#include <math/Quaternion.h>

#include <cmath>
#include <cstring>

Quaternion::Quaternion() : _q{0.0f, 0.0f, 0.0f} {}

Quaternion::Quaternion(float x, float y, float z, float w) : _q{x, y, z, w} {}

Quaternion::Quaternion(float scalar) : _q{0.0f, 0.0f, 0.0f, scalar} {}

Quaternion::Quaternion(const Vector3& vector) : 
    _q{vector[0], vector[1], vector[2], 0.0f} {}

Quaternion::Quaternion(const Vector3& vector, float scalar) : 
    _q{vector[0], vector[1], vector[2], scalar} {}

Quaternion::Quaternion(const Vector4& vector) : _q{vector} {}

// assignment operator
Quaternion& Quaternion::operator = (const Quaternion &other) {
    std::memcpy(&_q[0], &other[0], VEC4_SIZE);
    return *this;
}

bool Quaternion::operator ==(const Quaternion& other) const {
    return _q == other._q; 
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
    _q /= other._q;
    return *this;
}

Quaternion& Quaternion::operator *=(const Quaternion& other) {
    _q *= other._q;
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
    return Quaternion(axis.Normalize() * std::sin(angle), std::cos(angle)); 
}
    
Vector3 Quaternion::Axis() const {
    return {_q._v};
}

float Quaternion::Angle() const {
    return _q[3];
}

float Quaternion::Norm() const {
    return std::sqrt(_q[0] * _q[0] + _q[1] * _q[1] + _q[2] * _q[2] + _q[3] * _q[3]);
}

Quaternion Quaternion::Unit() const {
    return *this / Norm(); 
}

Quaternion Quaternion::Conjugate() const {
    return {-_q[0], -_q[1], -_q[2], _q[3]};
}

Quaternion Quaternion::Inverse() const {
    return Conjugate() / Norm();
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
    return inStream >> *((Vector4*)&quaternion); // POD convert to Vector4
}

std::ostream & operator << (std::ostream &outStream, const Quaternion &quaternion) {
    return outStream << *((Vector4*)&quaternion); 
}
