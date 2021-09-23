//
// Utility include for all vector types, plus some constant directions
//

#ifndef VECTORS_H
#define VECTORS_H 1

#include <math/Vector2.h>
#include <math/Vector3.h>
#include <math/Vector4.h>

// OpenGL directions
#define UP Vector3{0.0f, 1.0f, 0.0f}
#define DOWN Vector3{0.0f, -1.0f, 0.0f}
#define LEFT Vector3{-1.0f, 0.0f, 0.0f}
#define RIGHT Vector3{1.0f, 0.0f, 0.0f}
#define FORWARD Vector3{-1.0f, 0.0f, 0.0f} // camera points in -Z
#define BACKWARD Vector3{1.0f, 0.0f, 0.0f}

#endif // !VECTORS_H