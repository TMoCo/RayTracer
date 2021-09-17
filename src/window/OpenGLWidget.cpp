//
// OpenGLWidget class declaration
//

#include <window/OpenGLWidget.h>

OpenGLWidget::OpenGLWidget(const Model& m, QWidget* parent) : 
    QOpenGLWidget(parent), model{m}, transform{}, arcBall{} {

    transform.Translate({0.0f, 0.0f, -5.0f});
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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = (float)w / (float)h;
    glMultMatrixf(Matrix4::Perspective(45.0f, aspectRatio, 0.1f, 10.0f)[0]);
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glLoadIdentity();
    float pos[] = {2.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glPopMatrix();
    
    float color[] = {0.1f, 0.1f, 0.1f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, color);

    // set model view matrix based on arcball rotation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(transform.GetMatrix()[0]);
    glMultMatrixf(Matrix4::RotationMatrix(Quaternion::AngleAxis({0.0f, 1.0f, 0.0f}, RADIANS(90.0f)))[0]);

    model.Render();
}


void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
    arcBall.BeginDrag(scaleMousePos(event->x(), event->y()));
    transform.orientation = arcBall.GetOrientation();
    update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    arcBall.ContinueDrag(scaleMousePos(event->x(), event->y()));
    transform.orientation = arcBall.GetOrientation();
    update();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    arcBall.EndDrag(scaleMousePos(event->x(), event->y()));
    transform.orientation = arcBall.GetOrientation();
    update();
}

Vector2 OpenGLWidget::scaleMousePos(float x, float y) {
    float width = this->width(); 
    float height = this->height();
    // return x and y in [-1, -1] range
    return { (2.0f * x - width) / width, (height - 2.0f * y) / height };
}