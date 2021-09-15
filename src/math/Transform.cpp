//
// Transform class definition
//

#include <math/Transform.h>

Transform::Transform(const Vector3& p, const Quaternion& o) : position{p}, 
    orientation{o} {}

Matrix4 Transform::GetMatrix() const {
    return Matrix4::RotationMatrix(orientation) * Matrix4::TranslationMatrix(position);
}

void Transform::Rotate(const Quaternion& r) {
    orientation = orientation * r; // bad
}

void Transform::Translate(const Vector3& v) {
    position += v;
}
