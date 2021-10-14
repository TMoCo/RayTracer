//
// Transform class declaration
//

#ifndef TRANSFORM_H
#define TRANSFORM_H 1

#include <math/Matrix4.h>
#include <math/Quaternion.h>
#include <math/Vector3.h>

class Transform {
public:
    Vector3 translation;

    Quaternion orientation;

    Transform(const Vector3& p = {}, const Quaternion& o = {});

    Matrix4 toMatrix() const;

    // modify the transform
    void rotate(const Quaternion& r);

    void translate(const Vector3& v);

    Vector3 transformPoint(const Vector3& vec3) const;

    Vector3 rotatePoint(const Vector3& vec3) const;

    Vector3 translatePoint(const Vector3& vec3) const;
};

#endif // !TRANSFORM_H