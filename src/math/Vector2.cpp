//
// Vector2 class definition
//

#include <math/Vector2.h>

#include <cstring>

Vector2::Vector2(float X, float Y) : x(X), y(Y) {}

Vector2::Vector2(const Vector2& other) : _v{} {
    std::memcpy(_v, other._v, VEC2_SIZE);
}

float& Vector2::operator [](const uint32_t index) { // index greater than 1 not defined
    return _v[index];
}

const float& Vector2::operator [](const uint32_t index) const {
    return _v[index];
}

Vector2& Vector2::operator =(const Vector2& other) {
    std::memcpy(this, &other, VEC2_SIZE); // 2 * sizeof(float) = 8
    return *this;
}