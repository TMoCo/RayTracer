//
// Vector3 class definition
//

#include <core/core.h>

#include <math/Vector3.h>

#include <cmath>
#include <cstring>
#include <iomanip>

Vector3::Vector3() : _v{} {
}

Vector3::Vector3(F32 x, F32 y, F32 z) : _v{x, y, z} {}

Vector3::Vector3(const Vector3& other) : _v{} {
    std::memcpy(_v, other._v, SIZEOF_VEC3); // 3 * sizeof(F32) = 12 = 0xc
}

Vector3::Vector3(const F32* values) : _v{} {
    std::memcpy(_v, values, SIZEOF_VEC3);
}

Vector3& Vector3::operator =(const Vector3& other) {
    std::memcpy(_v, &other[0], SIZEOF_VEC3);
    return *this;
}

bool Vector3::operator ==(const Vector3& other) const {
    return std::memcmp(_v, &other[0], SIZEOF_VEC3);
}

Vector3& Vector3::operator +=(const Vector3& other) {
    _v4 = _mm_add_ps(_v4, other._v4);
    return *this;
}

Vector3& Vector3::operator -=(const Vector3& other) {
    _v4 = _mm_sub_ps(_v4, other._v4);
    return *this;
}

Vector3& Vector3::operator /=(const Vector3& other) {
    _v4 = _mm_div_ps(_v4, other._v4);
    return *this;
}

Vector3& Vector3::operator *=(const Vector3& other) {
    _v4 = _mm_mul_ps(_v4, other._v4);
    return *this;
}

Vector3& Vector3::operator /=(const F32& factor) {
    _v4 = _mm_div_ps(_v4, _mm_load1_ps(&factor));
    return *this;
}

Vector3& Vector3::operator *=(const F32& factor) {
    _v4 = _mm_mul_ps(_v4, _mm_load1_ps(&factor));
    return *this;
}

Vector3 Vector3::operator -() const {
    return *this * -1.0f;
}

F32& Vector3::operator [](const uint32_t index) { // index greater than 2 not defined
    return _v[index];
}

const F32& Vector3::operator [](const uint32_t index) const {
    return _v[index];
}

F32 Vector3::Dot(const Vector3& other) const {
    __m128 dot = _mm_mul_ps(_v4, other._v4);
    return dot[0] + dot[1] + dot[2] + dot[3];
}

/*
Vector3 Vector3::Cross(const Vector3& other) const {
    return {
        _v[1] * other[2] - _v[2] * other[1],
        _v[2] * other[0] - _v[0] * other[2], 
        _v[0] * other[1] - _v[1] * other[0]};
}
*/

Vector3 Vector3::Cross(const Vector3& other) const {
   __m128 res = _mm_sub_ps(
        _mm_mul_ps(
            _mm_shuffle_ps(_v4, _v4, _MM_SHUFFLE(3, 0, 2, 1)),
            _mm_shuffle_ps(other._v4, other._v4, _MM_SHUFFLE(3, 1, 0, 2))), 
        _mm_mul_ps(
            _mm_shuffle_ps(_v4, _v4, _MM_SHUFFLE(3, 1, 0, 2)),
            _mm_shuffle_ps(other._v4, other._v4, _MM_SHUFFLE(3, 0, 2, 1))));
    return Vector3{(float*)&res};
}



F32 Vector3::Length() const { 
#if (__cplusplus >= 201703L)
    return std::hypot(_v[0], _v[1], _v[2]);
#else
    return std::sqrt(x*x + y*y + z*z);
#endif
}

Vector3 Vector3::Normalize() const {
    return *this / Length();
}

Vector3 operator /(Vector3 lhs, const F32& rhs) {
    return lhs /= rhs;
}

Vector3 operator *(const F32& lhs, Vector3 rhs) {
    return rhs *= lhs;
}

Vector3 operator *(Vector3 lhs, const F32& rhs) {
    return lhs *= rhs;
}

std::istream& operator >> (std::istream &inStream, Vector3 &vector) {
    return inStream >> vector[0] >> vector[1] >> vector[2];
}

std::ostream & operator << (std::ostream &outStream, const Vector3 &vector) {
    return outStream << std::setprecision(4) << vector[0] << ' ' << std::setprecision(4) 
        << vector[1]<< ' ' << std::setprecision(4) << vector[2];
}