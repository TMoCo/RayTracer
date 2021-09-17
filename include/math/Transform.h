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
    Vector3 position;
    Quaternion orientation;

    Transform(const Vector3& p = {}, const Quaternion& o = {});

    Matrix4 GetMatrix() const;

    // modify the transform
    void Rotate(const Quaternion& r);
    void Translate(const Vector3& v);

    Vector3 TransformPoint(const Vector3& vec3) const;
    Vector3 RotatePoint(const Vector3& vec3) const;
    Vector3 TranslatePoint(const Vector3& vec3) const;
};

#endif // !TRANSFORM_H