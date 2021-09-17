//
// RenderWidget class declaration
//

#ifndef OPENGL_WIDGET_H
#define OPENGL_WIDGET_H 1

#include <math/Transform.h>
#include <model/Model.h>
#include <window/ArcBall.h>

#include <QOpenGLWidget>
#include <QtGui/QtEvents>

class OpenGLWidget : public QOpenGLWidget {
Q_OBJECT
private:
    const Model& model;

    Transform& transform;
    
    ArcBall arcBall;
public:
    OpenGLWidget(const Model& m, Transform& t, QWidget* parent);

    // qglwidget methods
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // mouse input
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    // scale mouse pos
    Vector2 scaleMousePos(float x, float y);
};

#endif // !OPENGL_WIDGET_H