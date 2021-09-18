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

    inline Vector3 At(F32 t) {
        return origin + t * direction;
    }
};

#endif // !RAY_H