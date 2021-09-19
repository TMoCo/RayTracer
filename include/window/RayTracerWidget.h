//
// Raytacer widget class declaration
//

#ifndef RAYTRACER_WIDGET_H
#define RAYTRACER_WIDGET_H 1

#include <image/Image.h>

#include <model/Model.h>

#include <math/Transform.h>

#include <raytracer/RayTracer.h>

#include <qt5/QtWidgets/QOpenGLWidget>
#include <qt5/QtWidgets/QtWidgets>

class RayTracerWidget : public QOpenGLWidget {
Q_OBJECT
private:
    Model* model;

    Transform& transform;

    RayTracer raytracer;

    Image<rgba_f> frameBuffer;

public:
    RayTracerWidget(QWidget* parent, Model* m, Transform& t);
    
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void RayTrace();
};

#endif // ! RAYTRACER_WIDGET_H