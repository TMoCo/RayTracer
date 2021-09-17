//
// Vector2 class declaration
//

#ifndef VECTOR2_H
#define VECTOR2_H 1

#include <cstdint>

#define VEC2_SIZE 0x8

class Vector2 {
public:
    union { 
        float _v[2]; 
        struct {
            float x, y;
        };
     };

    Vector2() {}
    Vector2(float X, float Y);
    Vector2(const Vector2& other);

    Vector2& operator =(const Vector2& other);

    // access operators
    float& operator [](const uint32_t index);
    const float& operator [](const uint32_t index) const;

    const float* ValuePtr() const; 
    float* ValuePtr();
};


#endif // ! VECTOR2_H