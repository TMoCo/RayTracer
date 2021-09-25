//
// Quaternion class definition
//

#include <core/core.h>

#include <math/Quaternion.h>

#include <cmath>
#include <cstring>

Quaternion::Quaternion(const Quaternion& other) : _q{other._q} {}

Quaternion::Quaternion(const Vector3& v, float s) : _vector{v} {
    _scalar = s;
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
    /*
    Vector3 v1{&_q[0]}, v2{&other._q[0]};
    float s1 = _q[3], s2 = other._q[3];
    std::cout << "\nnew rotation\n";
    std::cout << "scalars: " << s1 << " " << s2 << "\n" << _scalar << " " << other._scalar << "\n";
    std::cout << "vectors: " << v1 << " " << v2 << "\n" << _vector << " " << other._vector << "\n";
    std::cout << _scalar * other._scalar << " " <<  _vector.Dot(other._vector) << "\n";

    Quaternion q = Quaternion{s1 * v2 + s2 * v1 + v1.Cross(v2), s1 * s2 - v1.Dot(v2)}; 
    Quaternion p = Quaternion{
         _scalar * other._vector 
        + other._scalar * _vector 
        + _vector.Cross(other._vector), 
        _scalar * other._scalar - _vector.Dot(other._vector)
    };

    std::cout << q << "\n" << p << "\n\n";

    *this = p;
    */
    *this = Quaternion{
         _scalar * other._vector 
        + other._scalar * _vector 
        + _vector.Cross(other._vector), 
        _scalar * other._scalar - _vector.Dot(other._vector)
    };
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
    Vector3 t = (2.0f * quaternion._vector).Cross(vector);
    return vector + quaternion._scalar * t + quaternion._vector.Cross(t);
}

Vector3 Quaternion::Vector() const {
    return _vector;
}

float Quaternion::Scalar() const {
    return _scalar;
}

float Quaternion::Norm() const {
    return std::sqrt(_q.Dot(_q));
}

Quaternion Quaternion::Unit() const {
    return *this / Norm(); 
}

Quaternion Quaternion::Conjugate() const {
    return {-_vector, _scalar};
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
