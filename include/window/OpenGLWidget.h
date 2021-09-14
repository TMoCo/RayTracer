//
// RenderWidget class declaration
//

#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H 1

#include <math/Transform.h>
#include <model/Model.h>
#include <window/ArcBall.h>

#include <QOpenGLWidget>
#include <QtGui/QtEvents>

class OpenGLWidget : public QOpenGLWidget {
Q_OBJECT
private:
    const Model& model;

    Transform transform;
    
    ArcBall arcBall;

    int rotate = 0;

public:
    OpenGLWidget(const Model& m, QWidget* parent);

    // qglwidget methods
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // mouse input
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // !OPENGLWIDGET_H