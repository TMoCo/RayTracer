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
        glOrtho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
    else
        glOrtho(-1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio, -1.0f, 1.0f);
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set model view matrix based on arcball rotation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // arcball rotation
    Matrix4 mat{}; // get rotation here
    glMultMatrixf(mat[0]);
    
    model.Render();
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {

}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {

}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {

}