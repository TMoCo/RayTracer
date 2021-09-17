//
// Vector2 class declaration
//

#ifndef VECTOR2_H
#define VECTOR2_H 1

#include <math/types.h>

#define VEC2_SIZE 0x8

class Vector2 {
public:
    union { 
        F32 _v[2]; 
        struct {
            F32 x, y;
        };
     };

    Vector2() {}
    Vector2(F32 X, F32 Y);
    Vector2(const Vector2& other);

    Vector2& operator =(const Vector2& other);

    // access operators
    F32& operator [](const UI32 index);
    const F32& operator [](const UI32 index) const;

    const F32* ValuePtr() const; 
    F32* ValuePtr();
};


#endif // ! VECTOR2_H