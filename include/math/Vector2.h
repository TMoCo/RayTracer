//
// Vector2 class declaration
//

#ifndef VECTOR2_H
#define VECTOR2_H 1

#include <cstdint>

class Vector2 {
public:
    float x, y;

    Vector2() {}
    Vector2(float X, float Y) : x(X), y(Y) {}
    Vector2(const Vector2& other) : x(other.x), y(other.y) {}

    Vector2& operator =(const Vector2& other);

    // access operators
    float& operator [](const uint32_t index);
    const float& operator [](const uint32_t index) const;

    const float* ValuePtr() const; 
    float* ValuePtr();
};


#endif // ! VECTOR2_H