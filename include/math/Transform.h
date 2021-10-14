//
// Transform class declaration
//

#ifndef TRANSFORM_H
#define TRANSFORM_H 1

#include <math/Matrix4.h>
#include <math/Quaternion.h>
#include <math/Vector3.h>

struct Transform {
    Vector3 translation;

    Quaternion orientation;

    Transform(const Vector3& p = {}, const Quaternion& o = {}) : translation{ p }, orientation{ o } {}

    Matrix4 toWorldMatrix() const;

    Vector3 transformPoint(const Vector3& vec3) const;

    Vector3 rotate(const Vector3& vec3) const;

    Vector3 translate(const Vector3& vec3) const;

    // modify the transform
    static Transform transformChild(const Transform& parent, const Transform& child);
};

#endif // !TRANSFORM_H