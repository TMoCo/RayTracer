//
// RaytTracer widget class definition
//

#include <window/RayTracerWidget.h>


RayTracerWidget::RayTracerWidget(QWidget* parent, Model* m, Transform& t) : 
    QOpenGLWidget(parent), model(m), transform{t}, raytracer(), frameBuffer() {

    // 

}

void RayTracerWidget::initializeGL() {}

void RayTracerWidget::resizeGL(int w, int h) {
    frameBuffer.Resize(w, h);
}

void RayTracerWidget::paintGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    // display the image
    glDrawPixels(width(), height(), GL_RGBA, GL_FLOAT, frameBuffer[0]); 
}

void RayTracerWidget::RayTrace() {
    raytracer.RayTraceImage(frameBuffer, model, transform);
    update(); // call update to display result
}