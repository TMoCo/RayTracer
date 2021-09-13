//
// OpenGLWidget class declaration
//

#include <window/OpenGLWidget.h>

OpenGLWidget::OpenGLWidget(const Model& m, QWidget* parent) : 
    QOpenGLWidget(parent), model{m}, arcBall{} {}

void OpenGLWidget::initializeGL() {
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.9f, 0.9f, 0.7f, 1.0f);
}

void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = (float)w / (float)h;
    if (aspectRatio > 1.0f) // aspect ratio determines landscape or portrait
        glFrustum(-aspectRatio * 4.0f, aspectRatio * 4.0f, -4.0f, 4.0f, 0.1f, 10.0f);
    else
        glFrustum(-4.0f, 4.0f, -4.0f / aspectRatio, 4.0f / aspectRatio, 0.1f, 10.0f);
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_LIGHTING);

    glPushMatrix();
    float pos[] = {1.0f, 1.0f, 0.0f};
    float color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, color);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glPopMatrix();

    // set model view matrix based on arcball rotation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // arcball rotation
    /*
    Matrix4 mat{}; // get rotation here
    glMultMatrixf(mat[0]);
    */
    glTranslatef(0.0f, 0.0f, -0.5f);
    glRotatef(0.0F, 0.0f, 1.0f, 0.0f);
    model.Render();
}

void OpenGLWidget::Rotate() {
    rotate = (rotate++) & 359; // % 360
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {

}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {

}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {

}