//
// OpenGLWidget class declaration
//

#include <window/OpenGLWidget.h>

OpenGLWidget::OpenGLWidget(const Model& m, QWidget* parent) : 
    QOpenGLWidget(parent), model{m}, transform{}, arcBall{} {

    transform.Translate({0.0f, 0.0f, -2.0f});
}

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
        glOrtho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 10.0f);
    else
        glOrtho(-1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio, 1.0f, 10.0f);
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
    glTranslatef(0.0f, 0.0f, -2.0f);

    glMultMatrixf(transform.GetMatrix()[0]);

    model.Render();
}


void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
    float width = this->width(); 
    float height = this->height();
    float scaledX = (2.0 * event->x() - width) / width;
    float scaledY = (height - 2.0 * event->y()) / height; //qt coord to cartesian
    arcBall.BeginDrag(scaledX, scaledY);
    transform.orientation = arcBall.GetOrientation();
    update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    float x = event->x();
    float y = event->y();
    float width = this->width(); 
    float height = this->height();
    float scaledX = (2.0 * x - width) / width;
    float scaledY = (height - 2.0 * y) / height;
    arcBall.ContinueDrag(scaledX, scaledY);
    transform.orientation = arcBall.GetOrientation();
    std::cout << transform.orientation.Rotation() << '\n';
    update();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    float x = event->x();
    float y = event->y();
    float width = this->width(); 
    float height = this->height();
    float scaledX = (2.0 * x - width) / width;
    // this one has to flip from Qt coordinates to Cartesian
    float scaledY = (height - 2.0 * y) / height;
    // set the final rotation for the drag
    arcBall.EndDrag(scaledX, scaledY);
    transform.orientation = arcBall.GetOrientation();
    update();
}