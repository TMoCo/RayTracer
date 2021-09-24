//
// Window class declaration
//

#include <window/Window.h>

Window::Window(Model* model, const char* name) : QWidget(nullptr), 
    layout(nullptr), renderWidgets(nullptr), renderOptionsWidget(nullptr), 
    switchButton(nullptr) {
    setWindowTitle(QString(name));

    layout = new QGridLayout(this);

    renderWidgets = new RenderWidgets(this, model, &transform, &camera);
    renderOptionsWidget = new RenderOptionsWidget(this, &transform, &camera);
    
    switchButton = new QPushButton("Switch Render", this);

    layout->addWidget(renderWidgets, 0, 0);
    layout->addWidget(renderOptionsWidget, 0, 1);
    layout->addWidget(switchButton, 1, 0, 1, 2);

    QObject::connect(
        switchButton, SIGNAL(clicked()), 
        renderWidgets, SLOT(SwitchRender()));
    QObject::connect(
        renderWidgets, SIGNAL(SwitchedRender(int)), 
        renderOptionsWidget, SLOT(SwitchRender(int)));
    QObject::connect(
        renderOptionsWidget, SIGNAL(ShouldRayTrace(int)), 
        renderWidgets->raytracerWidget, SLOT(RayTrace(int)));
    QObject::connect(
        renderOptionsWidget, SIGNAL(ShouldSaveImage(QString)), 
        renderWidgets->raytracerWidget, SLOT(SaveImage(QString)));
    QObject::connect(
        renderOptionsWidget, SIGNAL(ShouldUpdateGl()), 
        renderWidgets->openGLWidget, SLOT(update()));
    QObject::connect(
        renderWidgets->openGLWidget, SIGNAL(resized()),
        renderOptionsWidget, SLOT(UpdateProperties()));
    QObject::connect(
        renderOptionsWidget, SIGNAL(ShouldResetAspectRatio()),
        renderWidgets, SLOT(ResetAspectRatio()));
}

Window::~Window() {
    // switch render mode
    delete switchButton;
    // render and options
    delete renderOptionsWidget;
    delete renderWidgets;
    // layout
    delete layout;
}
