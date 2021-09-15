//
// Quaternion class definition
//

#include <math/Quaternion.h>

#include <cmath>
#include <cstring>

Quaternion::Quaternion(const Vector3& v, float s) : _v{v}, _s{s} {}

Quaternion::Quaternion(float x, float y, float z, float w) : _v{x, y, z}, _s{w} {}

Quaternion::Quaternion(const float* values) : _v{values}, _s{values[3]} {}

Quaternion::Quaternion(const Vector4& vector) : _v{&vector[0]}, _s{vector[3]} {}

// assignment operator
Quaternion& Quaternion::operator = (const Quaternion &other) {
    std::memcpy(this, &other, VEC4_SIZE);
    return *this;
}

bool Quaternion::operator ==(const Quaternion& other) const {
    return std::memcmp(this, &other, VEC4_SIZE) == 0; 
}

Quaternion& Quaternion::operator +=(const Quaternion& other) {
    _v += other._v;
    _s += other._s;
    return *this;
}

Quaternion& Quaternion::operator -=(const Quaternion& other) {
    _v -= other._v;
    _s -= other._s;
    return *this;
}

Quaternion& Quaternion::operator /=(const Quaternion& other) {
    _v /= other._v;
    _s /= other._s;
    return *this;
}

Quaternion& Quaternion::operator *=(const Quaternion& other) {
    *this = Quaternion{
        _s * other._v - other._s * _v + _v.Cross(other._v), // vector
        _s * other._s + _v.Dot(other._v)}; // scalar
    return *this;
}

Quaternion& Quaternion::operator /=(const float& factor) {
    _v /= factor;
    return *this;
}

Quaternion& Quaternion::operator *=(const float& factor) {
    _v *= factor;
    return *this;
}

Quaternion& Quaternion::operator -() {
    return *this *= -1.0f;
}

float& Quaternion::operator [](const uint32_t index) {
    return ((float*)this)[index]; // run some tests on this...
}

const float& Quaternion::operator [](const uint32_t index) const {
    return ((float*)this)[index];
}

Quaternion Quaternion::AngleAxis(const Vector3& axis, float angle) {
    return Quaternion(axis.Normalize() * std::sin(angle), std::cos(angle)); 
}
    
Vector3 Quaternion::Axis() const {
    return _v;
}

float Quaternion::Angle() const {
    return _s;
}

float Quaternion::Norm() const {
    return std::sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _s * _s);
}

Quaternion Quaternion::Unit() const {
    return *this / Norm(); 
}

Quaternion Quaternion::Conjugate() const {
    return {-_v, _s};
}

Quaternion Quaternion::Inverse() const {
    return Conjugate() / (_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _s * _s);
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
