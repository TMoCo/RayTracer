//
// RenderWidgets class declaration
// 

#ifndef RENDERWIDGETS_H
#define RENDERWIDGETS_H 1

#include <render/Camera.h>

#include <model/Model.h>

#include <window/OpenGLWidget.h>
#include <window/RayTracerWidget.h>

#include <qt5/QtWidgets/QtWidgets>

class RenderWidgets : public QWidget {
Q_OBJECT
private:
    QStackedLayout* layout;
    
    OpenGLWidget* openGLWidget;
    RayTracerWidget* raytracerWidget;

    Model* model;
    Transform transform;
    Camera camera;
    
public:
    RenderWidgets(QWidget* parent, Model* m);
    ~RenderWidgets();

public slots:
    void SwitchRender();
    void RayTrace();
    void SaveImage(QString name);

signals:
    void SwitchedRender(int index);
};

#endif