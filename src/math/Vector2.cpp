//
// Vector2 class definition
//

#include <math/Vector2.h>

#include <cstring>

float& Vector2::operator [](const uint32_t index) { // index greater than 1 not defined
    return *(ValuePtr() + index);
}

const float& Vector2::operator [](const uint32_t index) const {
    return *(ValuePtr() + index);
}

Vector2& Vector2::operator =(const Vector2& other) {
    std::memcpy(this, &other, 0x8); // 2 * sizeof(float) = 8
    return *this;
}

const float* Vector2::ValuePtr() const {
    return ((float*)this);
}

float* Vector2::ValuePtr() {
    return ((float*)this);
}