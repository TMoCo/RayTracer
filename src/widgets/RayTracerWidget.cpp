//
// RaytTracer widget class definition
//
/*
#include <window/RayTracerWidget.h>

#include <fstream>

RayTracerWidget::RayTracerWidget(QWidget* parent, Model* m, Transform* t, 
    Camera* c) : 
    QOpenGLWidget(parent), model(m), transform{t}, camera{c}, 
    raytracer(), frameBuffer() {}

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

void RayTracerWidget::RayTrace(int samples) {
    raytracer.RayTraceImage(frameBuffer, model, *transform, camera, samples);
    update(); // call update to display result
}

void RayTracerWidget::SaveImage(QString name) {
    // create name
    char path[MAX_NAME_LENGTH] = "../screenshots/";
    std::strcat(path, name.toStdString().data());
    std::strcat(path, ".ppm");
    
    std::ofstream out(path);
    
    frameBuffer.ToPPM(out);
}
*/