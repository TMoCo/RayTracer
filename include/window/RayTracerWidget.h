//
// Raytacer widget class declaration
//

#ifndef RAYTRACER_WIDGET_H
#define RAYTRACER_WIDGET_H 1

#include <image/Image.h>
#include <model/Model.h>

#include <QOpenGLWidget>

class RayTracer{}; // place holder class

class RayTracerWidget : public QOpenGLWidget {
Q_OBJECT
private:
    RayTracer raytracer;

    const Model& model;

    Image<rgba_s> frameBuffer;

public:
    RayTracerWidget(const Model& model, QWidget* parent);
    
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void RayTrace();
};

#endif // ! RAYTRACER_WIDGET_H