//
// RendeWidgets class definition
//

#include <window/RenderWidgets.h>

RenderWidgets::RenderWidgets(const Model& m, QWidget* parent) : QWidget(parent),
    layout(nullptr), openGLWidget(nullptr), raytracerWidget(nullptr), model(m) {
    layout = new QStackedLayout(this);
    
    openGLWidget = new OpenGLWidget(model, this);
    raytracerWidget = new RayTracerWidget(model, this);

    // add widgets in right order
    layout->addWidget(openGLWidget);
    layout->addWidget(raytracerWidget);
}

RenderWidgets::~RenderWidgets() {
    delete openGLWidget;
    delete raytracerWidget;
    delete layout;
}

void RenderWidgets::SwitchWidget() {
    layout->setCurrentIndex((layout->currentIndex() + 1) & 1);
}