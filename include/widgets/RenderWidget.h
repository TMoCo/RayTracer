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
    
    Camera* camera;
public:
    OpenGLWidget* openGLWidget;
    RayTracerWidget* raytracerWidget;
    
    RenderWidgets(QWidget* parent, Model* model, Transform* transform, 
        Camera* camera);
    ~RenderWidgets();

public slots:
    void ResetAspectRatio();
    void SwitchRender();

signals:
    void SwitchedRender(int index);
};

#endif