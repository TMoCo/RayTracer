//
// ArcBall class definition
//

#include <window/ArcBall.h>

#include <cmath>

ArcBall::ArcBall() : base{0.0f, 0.0f, 0.0f, 1.0f}, 
    current{0.0f, 0.0f, 0.0f, 1.0f}, drag{0.0f, 0.0f, 1.0f, 0.0f} {}

Quaternion ArcBall::FindQuat(float x, float y) {
    float d2 = x*x + y*y;
    if (d2 > 1.0f) {
        return Quaternion{Vector3{x, y, 0.0f} / std::sqrt(d2), 1.0f};
    }
    else {
        return Quaternion{Vector3{x, y, std::sqrt(1.0f - d2)}, 1.0f};
    }
}

Quaternion ArcBall::GetOrientation() {
    return current * base;
}

Matrix4 ArcBall::GetOrientationMatrix() {
    return (current * base).Rotation();
}

void ArcBall::BeginDrag(float x, float y) {
    std::cout << base << '\n';
    drag = FindQuat(x, y);
}
    
void ArcBall::ContinueDrag(float x, float y) {
    current = FindQuat(x, y) * drag.Inverse();
}

void ArcBall::EndDrag(float x, float y) {
    ContinueDrag(x, y); // update current

    // reset quaternions for next arcball use
    drag = {0.0f, 0.0f, 1.0f, 0.0f};
    base = current * base;
    current = {0.0f, 0.0f, 0.0f, 1.0f};
}