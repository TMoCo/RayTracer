/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// ArcBall class declaration
//

#ifndef ARCBALL_H
#define ARCBALL_H 1

#include <Quaternion.h>
#include <Vector2.h>

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

#endif // !ARCBALL_H
