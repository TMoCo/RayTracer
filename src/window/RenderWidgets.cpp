//
// RendeWidgets class definition
//

#include <window/RenderWidgets.h>

RenderWidgets::RenderWidgets(QWidget* parent, Model* model, Transform* transform, 
        Camera* camera) : QWidget(parent) {
    
    layout = new QStackedLayout(this);

    openGLWidget = new OpenGLWidget(this, model, transform, camera);
    raytracerWidget = new RayTracerWidget(this, model, *transform);

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