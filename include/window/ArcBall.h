//
// ArcBall class declaration
//

#ifndef ARCBALL_H
#define ARCBALL_H 1

#include <math/Quaternion.h>

class ArcBall {
public:
    Quaternion base;
    Quaternion current;
    Quaternion drag;

    ArcBall();

    Quaternion FindQuat(float x, float y);

    void BeginDrag(float x, float y);
    void ContinueDrag(float x, float y);
    void EndDrag(float x, float y);
};

#endif // !ARCBALL_H