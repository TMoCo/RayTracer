//
// RaytTracer widget class definition
//

#include <window/RayTracerWidget.h>


RayTracerWidget::RayTracerWidget(const Model& model, QWidget* parent) : 
    QOpenGLWidget(parent), raytracer(), model(model), frameBuffer() {

}

void RayTracerWidget::initializeGL() {

}

void RayTracerWidget::resizeGL(int w, int h) {
    frameBuffer.Resize(w, h);
}

void RayTracerWidget::paintGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    // and display the image
    glDrawPixels(width(), height(), GL_RGBA, GL_FLOAT, frameBuffer[0]); 
}

void RayTracerWidget::RayTrace() {
    update(); // calls update on finish
}