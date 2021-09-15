//
// Raytacer widget class declaration
//

#ifndef RAYTRACER_H
#define RAYTRACER_H 1

#include <model/Model.h>

#include <QOpenGLWidget>

class RayTracer{}; // place holder class

class RaytacerWidget : public QOpenGLWidget {
Q_OBJECT
private:
    RayTracer raytracer;

    const Model& model;

public:
    RaytacerWidget(QWidget* parent);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:
    void RayTrace();
};

#endif