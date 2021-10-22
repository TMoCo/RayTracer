//
// Raytacer widget class declaration
//

/*
#ifndef RAYTRACER_WIDGET_H_
#define RAYTRACER_WIDGET_H_ 1

#include <image/Image.h>

#include <model/Model.h>

#include <math/Transform.h>

#include <render/Camera.h>

#include <raytracer/RayTracer.h>

#include <qt5/QtWidgets/QOpenGLWidget>
#include <qt5/QtWidgets/QtWidgets>

class RayTracerWidget : public QOpenGLWidget {
Q_OBJECT
private:
    Model* model;
    Transform* transform;
    Camera* camera;

    RayTracer raytracer;

    Image framebuffer;

public:
    RayTracerWidget(QWidget* parent, Model* m, Transform* t, Camera* c);
    
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void RayTrace(int samples);
    void SaveImage(QString name);
};

#endif // ! RAYTRACER_WIDGET_H_
*/