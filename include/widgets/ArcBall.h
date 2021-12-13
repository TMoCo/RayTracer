/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// ArcBall class declaration
//

#ifndef ARCBALL_H_
#define ARCBALL_H_ 1

#include <math/Quaternion.h>
#include <math/Vector2.h>

class ArcBall {
public:
    Quaternion base;
    Quaternion current;
    Quaternion drag;

    ArcBall();

    Quaternion FindQuat(const Vector2& v);

    Quaternion GetOrientation();

    void BeginDrag(const Vector2& v);
    void ContinueDrag(const Vector2& v);
    void EndDrag(const Vector2& v);
};

#endif // !ARCBALL_H_
