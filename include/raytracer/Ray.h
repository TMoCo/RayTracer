//
// Ray struct declaration
//

#ifndef RAY_H
#define RAY_H

#include <math/Vector3.h>

struct Ray {
public:
    Vector3 origin;
    Vector3 direction;

    Ray(const Vector3& o = {}, const Vector3& d = {}) : origin{o}, direction{d} {}

    inline Vector3 At(F32 t) const {
        return origin + t * direction;
    }
};

#endif // !RAY_H