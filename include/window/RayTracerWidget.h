//
// Raytacer widget class declaration
//

#ifndef RAYTRACER_WIDGET_H
#define RAYTRACER_WIDGET_H 1

#include <image/Image.h>
#include <model/Model.h>
#include <raytracer/RayTracer.h>

#include <QOpenGLWidget>

class RayTracerWidget : public QOpenGLWidget {
Q_OBJECT
private:
    RayTracer raytracer;

    const Model& model;

    Transform& transform;

    Image<rgba_s> frameBuffer;

public:
    RayTracerWidget(const Model& m, Transform& t, QWidget* parent);
    
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void RayTrace();
};

#endif // ! RAYTRACER_WIDGET_H