//
// ArcBall class definition
//

#include <widgets/ArcBall.h>

#include <cmath>

ArcBall::ArcBall() : base{0.0f, 0.0f, 0.0f, 1.0f}, 
    current{0.0f, 0.0f, 0.0f, 1.0f}, drag{0.0f, 0.0f, 1.0f, 0.0f} {}

Quaternion ArcBall::FindQuat(const Vector2& v) {
    float d2 = v[0]*v[0] + v[1]*v[1];
    if (d2 > 1.0f) { // point outside unit sphere?
        return Quaternion{Vector3{v[0], v[1], 0.0f} / std::sqrt(d2), 0.0f};
    }
    else {
        return Quaternion{Vector3{v[0], v[1], std::sqrt(1.0f - d2)}, 0.0f};
    }
}

Quaternion ArcBall::GetOrientation() {
    return current * base;
}

void ArcBall::BeginDrag(const Vector2& v) {
    drag = FindQuat(v);
}
    
void ArcBall::ContinueDrag(const Vector2& v) {
    current = FindQuat(v) * drag.inverse();
}

void ArcBall::EndDrag(const Vector2& v) {
    ContinueDrag(v); // update current
    // reset quaternions for next arcball use
    drag = {0.0f, 0.0f, 1.0f, 0.0f};
    base = current * base;
    current = {0.0f, 0.0f, 0.0f, 1.0f};
}