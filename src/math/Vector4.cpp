//
// Vector4 class definition
//

#include <math/Vector4.h>

#include <cmath>
#include <iomanip>

Vector4::Vector4() : _v{} {}

Vector4::Vector4(float x, float y, float z, float w) : _v{} {
    _v.x = x;
    _v.y = y;
    _v.z = z;
    _v.w = w;
}

Vector4::Vector4(const float* v) : _v{} {
    std::memcpy(&_v, v, VEC4_SIZE); // 4 * si_v[2]eof(float) = 16 = 0_v[0]10
}

Vector4::Vector4(const Vector4& other) : _v{} {
    std::memcpy(&_v, &other[0], VEC4_SIZE);
}
Vector4& Vector4::operator =(const Vector4& other) {
    std::memcpy(this, &other[0], VEC4_SIZE); 
    return *this;
}

bool Vector4::operator ==(const Vector4& other) const {
    return memcmp(&_v, &other[0], VEC4_SIZE) == 0;
}

Vector4& Vector4::operator +=(const Vector4& other) {
    _v.v4 = _mm_add_ps(_v.v4, other._v.v4);
    return *this;    
}

Vector4& Vector4::operator -=(const Vector4& other) {
    _v.v4 = _mm_sub_ps(_v.v4, other._v.v4);
    return *this;
}

Vector4& Vector4::operator /=(const Vector4& other) {
    _v.v4 = _mm_div_ps(_v.v4, other._v.v4);
    return *this;
}

Vector4& Vector4::operator *=(const Vector4& other) {
    _v.v4 = _mm_mul_ps(_v.v4, other._v.v4);
    return *this;
}

Vector4& Vector4::operator /=(const float& factor) {
    _v.v4 = _mm_div_ps(_v.v4, _mm_load1_ps(&factor));
    return *this;
}

Vector4& Vector4::operator *=(const float& factor) {
    _v = _mm_mul_ps(_v.v4, _mm_load1_ps(&factor));
    return *this;
}

Vector4& Vector4::operator -() {
    return *this *= -1.0f;
}

float& Vector4::operator [](const uint32_t index) {
    return _v[index];
}

const float& Vector4::operator [](const uint32_t index) const {
    return _v[index];
}

Vector3 Vector4::Point3D() const {
    float w = 1.0f / _v[3];
    return {_v[0] * w , _v[1] * w, _v[2] * w};
}

Vector3 Vector4::ToVector3() const {
    return {_v[0], _v[1], _v[2]};
}

float Vector4::Dot(const Vector4& other) const {
    vec4 dot = _mm_mul_ps(_v.v4, other._v.v4);
    return dot[0] + dot[1] + dot[2] + dot[3];
}

float Vector4::Length() const {
    return std::sqrt(Dot(*this));
}

Vector4 Vector4::Normalize() const {
    return *this / Length();
}

float Vector4::Sum() const {
    return _v[0] + _v[1] + _v[2] + _v[3];
}

Vector4 operator /(Vector4 lhs, const float rhs) {
    return lhs /= rhs;
}

Vector4 operator *(Vector4 rhs, const float lhs) {
    return rhs *= lhs;
}

Vector4 operator *(const float lhs, Vector4& rhs) {
    return rhs * lhs;
}


std::istream& operator >> (std::istream &inStream, Vector4 &vector) {
    return inStream >> vector[0] >> vector[1] >> vector[2] >> vector[3];
}

std::ostream & operator << (std::ostream &outStream, const Vector4 &vector) {
    return outStream << std::setprecision(4) << vector[0] << ' ' 
    << std::setprecision(4) << vector[1] << ' ' 
    << std::setprecision(4) << vector[2] << ' ' 
    << std::setprecision(4) << vector[3];
}