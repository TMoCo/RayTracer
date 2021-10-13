//
// RenderWidget class declaration
//

/*
#ifndef OPENGL_WIDGET_H
#define OPENGL_WIDGET_H 1

#include <render/Camera.h>

#include <math/Transform.h>

#include <model/Model.h>

#include <window/ArcBall.h>

#include <qt5/QtWidgets/QOpenGLWidget>
#include <qt5/QtGui/QtEvents>

class OpenGLWidget : public QOpenGLWidget {
Q_OBJECT
private:
    Model* model;

    Transform* transform;
    Camera* camera;
    
    ArcBall arcBall;
public:
    OpenGLWidget(QWidget* parent, Model* m, Transform* t, Camera* c);

    // qglwidget methods
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // mouse input
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    // scale mouse pos
    Vector2 scaleMousePos(F32 x, F32 y);
};

#endif // !OPENGL_WIDGET_H
*/