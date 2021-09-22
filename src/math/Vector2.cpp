//
// Vector2 class definition
//

#include <math/Vector2.h>

#include <cstring>

Vector2::Vector2(F32 X, F32 Y) : x(X), y(Y) {}

Vector2::Vector2(const Vector2& other) : _v{} {
    std::memcpy(_v, other._v, VEC2_SIZE);
}

F32& Vector2::operator [](const UI32 index) { // index greater than 1 not defined
    return _v[index];
}

const F32& Vector2::operator [](const UI32 index) const {
    return _v[index];
}

Vector2& Vector2::operator =(const Vector2& other) {
    std::memcpy(this, &other, VEC2_SIZE); // 2 * sizeof(F32) = 8
    return *this;
}

Vector2& Vector2::operator -=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2& Vector2::operator +=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2& Vector2::operator /=(const F32& factor) {
    F32 inv = 1.0f / factor;
    x *= inv;
    y *= inv;
    return *this;
}

Vector2 operator *(Vector2 lhs, const F32& rhs) {
    return Vector2{lhs[0] * rhs, lhs[1] * rhs};
}

Vector2 operator /(Vector2 lhs, const F32& rhs) {
    return lhs /= rhs;
}


Vector2 operator *(const F32& lhs, Vector2 rhs) {
    return Vector2{rhs[0] * lhs, rhs[1] * lhs};
}