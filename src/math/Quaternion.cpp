//
// Quaternion class definition
//

#include <math/Quaternion.h>

#include <cmath>
#include <cstring>

Quaternion::Quaternion(const Vector3& v, float s) : _q{&v[0]} {
    _q._v.w = s;
}

Quaternion::Quaternion(float x, float y, float z, float w) : _q{x, y, z, w} {}

Quaternion::Quaternion(const float* values) : _q{values} {}

Quaternion::Quaternion(const Vector4& vector) : _q{vector} {}

// assignment operator
Quaternion& Quaternion::operator = (const Quaternion &other) {
    std::memcpy(this, &other, VEC4_SIZE);
    return *this;
}

bool Quaternion::operator ==(const Quaternion& other) const {
    return std::memcmp(this, &other, VEC4_SIZE) == 0; 
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
    *this = Quaternion{s1 * v2 - s2 * v1 + v1.Cross(v2), s1 * s2 + v1.Dot(v2)}; // must be a better way
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
    return ((float*)this)[index]; // run some tests on this...
}

const float& Quaternion::operator [](const uint32_t index) const {
    return ((float*)this)[index];
}

Quaternion Quaternion::AngleAxis(const Vector3& axis, float angle) {
    return Quaternion(axis.Normalize() * std::sin(angle), std::cos(angle)); 
}
    
Vector3 Quaternion::Axis() const {
    return &_q[0];
}

float Quaternion::Angle() const {
    return _q[3];
}

float Quaternion::Norm() const {
    return std::sqrt(_q.Dot(_q));
}

Quaternion Quaternion::Unit() const {
    return *this / Norm(); 
}

Quaternion Quaternion::Conjugate() const {
    Quaternion p{_q * -1.0f};
    p[3] = _q[3];
    return p;
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
    return inStream >> *((Vector4*)&quaternion); // POD convert to Vector4
}

std::ostream & operator << (std::ostream &outStream, const Quaternion &quaternion) {
    return outStream << *((Vector4*)&quaternion); 
}
