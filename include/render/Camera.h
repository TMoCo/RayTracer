//
// Camera class declaration
//

#ifndef CAMERA_H
#define CAMERA_H 1

#include <core/types.h>

#include <math/Transform.h>
#include <math/Vectors.h>

#include <cmath>

// basic camera class 
class Camera {
public:
    F32 aspectRatio;
    F32 FOV;
    F32 zNear;
    F32 zFar;
    // transform to camera space
    Transform transform;

    Camera() : aspectRatio(0.0f), FOV(0.0f), zNear(0.0f), zFar(0.0f) {}

    // given pixel normalised device coordinates
    inline Vector3 GenerateRay(const Vector2& PNDC) {
        // fov
        F32 tanHalfFOV = std::tan(FOV * 0.5f);
        // return camera ray
        return {
            (2.0f * PNDC.x - 1.0f) * aspectRatio * tanHalfFOV,
            (2.0f * PNDC.y - 1.0f) * tanHalfFOV,
            zNear
        };
    }
};

#endif // !CAMERA_H