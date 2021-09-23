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
    orientation *= r;
}

void Transform::Translate(const Vector3& v) {
    position += v;
}

Vector3 Transform::TransformPoint(const Vector3& vec3) const {
    return (GetMatrix() * Vector4{vec3})._v;
}

Vector3 Transform::RotatePoint(const Vector3& vec3) const {
    return (Matrix4::RotationMatrix(orientation) * Vector4{vec3})._v;
}

Vector3 Transform::TranslatePoint(const Vector3& vec3) const {
    return (Matrix4::TranslationMatrix(position) * Vector4{vec3})._v;
}
