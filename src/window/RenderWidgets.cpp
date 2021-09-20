//
// RendeWidgets class definition
//

#include <window/RenderWidgets.h>

RenderWidgets::RenderWidgets(QWidget* parent, Model* m) : QWidget(parent),
    layout(nullptr), openGLWidget(nullptr), raytracerWidget(nullptr), model(m), transform{} {
    
    layout = new QStackedLayout(this);

    openGLWidget = new OpenGLWidget(this, model, transform, camera);
    raytracerWidget = new RayTracerWidget(this, model, transform);

    // add widgets in right order
    layout->addWidget(openGLWidget);
    layout->addWidget(raytracerWidget);
}

RenderWidgets::~RenderWidgets() {
    // delete widgets
    delete raytracerWidget;
    delete openGLWidget;
    // delete layout
    delete layout;

}

void RenderWidgets::SwitchRender() {
    layout->setCurrentIndex((layout->currentIndex() + 1) & 1);
    emit SwitchedRender(layout->currentIndex());
}

void RenderWidgets::RayTrace() {
    raytracerWidget->RayTrace(); // calls update method
}

void RenderWidgets::SaveImage(QString name) {
    raytracerWidget->SaveImage(name.toStdString().data());
}