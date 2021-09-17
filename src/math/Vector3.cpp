//
// Vector3 class definition
//

#include <math/Vector3.h>

#include <cmath>
#include <cstring>
#include <iomanip>

Vector3::Vector3() : _v{} {
    std::memset(&_v, 0, VEC3_SIZE);
}

Vector3::Vector3(float x, float y, float z) : _v{x, y, z} {}

Vector3::Vector3(const Vector3& other) : _v{} {
    std::memcpy(_v, other._v, VEC3_SIZE); // 3 * sizeof(float) = 12 = 0xc
}

Vector3::Vector3(const float* values) : _v{} {
    std::memcpy(_v, values, VEC3_SIZE);
}

Vector3& Vector3::operator =(const Vector3& other) {
    std::memcpy(_v, &other[0], VEC3_SIZE);
    return *this;
}

bool Vector3::operator ==(const Vector3& other) const {
    return std::memcmp(_v, &other[0], VEC3_SIZE);
}

Vector3& Vector3::operator +=(const Vector3& other) {
    _v[0] += other[0];
    _v[1] += other[1];
    _v[2] += other[2];
    return *this;
}

Vector3& Vector3::operator -=(const Vector3& other) {
    _v[0] -= other[0];
    _v[1] -= other[1];
    _v[2] -= other[2];
    return *this;
}

Vector3& Vector3::operator /=(const Vector3& other) {
    _v[0] /= other[0];
    _v[1] /= other[1];
    _v[2] /= other[2];
    return *this;
}

Vector3& Vector3::operator *=(const Vector3& other) {
    _v[0] *= other[0];
    _v[1] *= other[1];
    _v[2] *= other[2];
    return *this;
}

Vector3& Vector3::operator /=(const float& factor) {
    float inv = 1.0f / factor;
    _v[0] *= inv;
    _v[1] *= inv;
    _v[2] *= inv;
    return *this;
}

Vector3& Vector3::operator *=(const float& factor) {
    _v[0] *= factor;
    _v[1] *= factor;
    _v[2] *= factor;
    return *this;
}

Vector3 Vector3::operator -() const {
    return *this * -1.0f;
}

float& Vector3::operator [](const uint32_t index) { // index greater than 2 not defined
    return _v[index];
}

const float& Vector3::operator [](const uint32_t index) const {
    return _v[index];
}

float Vector3::Dot(const Vector3& other) const {
    return _v[0] * other[0] + _v[1] * other[1] + _v[2] * other[2];
}

Vector3 Vector3::Cross(const Vector3& other) const {
    return {
        _v[1] * other[2] - _v[2] * other[1],
        _v[2] * other[0] - _v[0] * other[2], 
        _v[0] * other[1] - _v[1] * other[0]};
}

float Vector3::Length() const { 
#if (__cplusplus >= 201703L)
    return std::hypot(_v[0], _v[1], _v[2]);
#else
    return std::sqrt(x*x + y*y + z*z);
#endif
}

Vector3 Vector3::Normalize() const {
    return *this / Length();
}

Vector3 operator /(Vector3 lhs, const float& rhs) {
    return lhs /= rhs;
}

Vector3 operator *(const float& lhs, Vector3 rhs) {
    return rhs *= lhs;
}

Vector3 operator *(Vector3 lhs, const float& rhs) {
    return lhs *= rhs;
}


std::istream& operator >> (std::istream &inStream, Vector3 &vector) {
    return inStream >> vector[0] >> vector[1] >> vector[2];
}

std::ostream & operator << (std::ostream &outStream, const Vector3 &vector) {
    return outStream << std::setprecision(4) << vector[0] << ' ' << std::setprecision(4) 
        << vector[1]<< ' ' << std::setprecision(4) << vector[2];
}