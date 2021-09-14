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

    void Rotate(const Quaternion& r);
    void Translate(const Vector3& v);
};

#endif // !TRANSFORM_H