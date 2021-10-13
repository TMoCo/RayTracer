//
// RendeWidgets class definition
//

/*
#include <window/RenderWidgets.h>

RenderWidgets::RenderWidgets(QWidget* parent, Model* m, Transform* t, Camera* c) 
    : QWidget(parent), camera(c) {
    
    layout = new QStackedLayout(this);

    openGLWidget = new OpenGLWidget(this, m, t, camera);
    raytracerWidget = new RayTracerWidget(this, m, t, camera);

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

void RenderWidgets::ResetAspectRatio() {
    camera->aspectRatio = (F32)width() / (F32)height(); 
    openGLWidget->update(); // reflect change in aspect ratio
}

void RenderWidgets::SwitchRender() {
    layout->setCurrentIndex((layout->currentIndex() + 1) & 1);
    emit SwitchedRender(layout->currentIndex());
}
*/