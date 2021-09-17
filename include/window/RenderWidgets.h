//
// RenderWidgets class declaration
// 

#ifndef RENDERWIDGETS_H
#define RENDERWIDGETS_H 1

#include <model/Model.h>
#include <window/OpenGLWidget.h>
#include <window/RayTracerWidget.h>

#include <QtWidgets>

class RenderWidgets : public QWidget {
Q_OBJECT
private:
    QStackedLayout* layout;
    OpenGLWidget* openGLWidget;
    RayTracerWidget* raytracerWidget; // to implement

    const Model& model;
public:
    RenderWidgets(const Model& m, QWidget* parent);
    ~RenderWidgets();

public slots:
    void SwitchWidget();
};

#endif