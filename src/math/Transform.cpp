//
// Transform class definition
//

#include <math/Transform.h>

Transform::Transform(const Vector3& p, const Quaternion& o) : position{p}, 
    orientation{o} {}

Matrix4 Transform::GetMatrix() const {
    Matrix4 t = Matrix4::Identity();
    // translate
    Matrix4::Translate(t, position);
    // rotate
    t = orientation.Rotation() * t;
    // scale
    // t.scale
    return t;
}

void Transform::Rotate(const Quaternion& r) {
    orientation = r * orientation;
}

void Transform::Translate(const Vector3& v) {
    position += v;
}
