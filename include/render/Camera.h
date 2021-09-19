//
// Camera class declaration
//

#ifndef CAMERA_H
#define CAMERA_H

#include <core/types.h>

// basic camera class 
class Camera {
public:
    F32 aspectRatio;
    F32 FOV;

    Camera() : aspectRatio(0.0f), FOV(0.0f) {}
};

#endif // !CAMERA_H