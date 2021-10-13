//
// OpenGLWidget class declaration
//

/*
#include <window/OpenGLWidget.h>

OpenGLWidget::OpenGLWidget(QWidget* parent, Model* m, Transform* t, Camera* c) : 
    QOpenGLWidget(parent), model{m}, transform{t}, camera{c}, arcBall{} {

    transform->Translate({0.0f, 0.0f, -3.0f});
}

void OpenGLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_AMBIENT, GL_FALSE);
    glClearColor(0.9f, 0.9f, 0.7f, 1.0f);
}

void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    camera->aspectRatio = (F32)w / (F32)h;
}

void OpenGLWidget::paintGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(Matrix4::Perspective(
        camera->FOV, camera->aspectRatio, camera->zNear, camera->zFar)._m);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glLoadIdentity();
    F32 pos[] = {2.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glPopMatrix();
    
    F32 color[] = {0.1f, 0.1f, 0.1f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, color);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // rotated by arcball rotation
    glMultMatrixf(transform->GetMatrix()._m);

    model->Render();
}



void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
    arcBall.BeginDrag(scaleMousePos(event->x(), event->y()));
    transform->orientation = arcBall.GetOrientation();
    update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    arcBall.ContinueDrag(scaleMousePos(event->x(), event->y()));
    transform->orientation = arcBall.GetOrientation();
    update();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    arcBall.EndDrag(scaleMousePos(event->x(), event->y()));
    transform->orientation = arcBall.GetOrientation();
    update();
}

Vector2 OpenGLWidget::scaleMousePos(F32 x, F32 y) {
    F32 width = this->width(); 
    F32 height = this->height();
    // return x and y in [-1, -1] range
    return { (2.0f * x - width) / width, (height - 2.0f * y) / height };
}
*/