//
// Window class declaration
//

#include <window/Window.h>

Window::Window(Model* model, const char* name) : QWidget(nullptr), 
    layout(nullptr), renderWidgets(nullptr), renderOptionsWidget(nullptr), 
    switchButton(nullptr) {
    setWindowTitle(QString(name));

    layout = new QGridLayout(this);

    renderWidgets = new RenderWidgets(this, model);
    renderOptionsWidget = new RenderOptionsWidget(this, transform, camera);
    
    switchButton = new QPushButton("Switch Render", this);

    layout->addWidget(renderWidgets, 0, 0);
    layout->addWidget(renderOptionsWidget, 0, 1);
    layout->addWidget(switchButton, 1, 0, 1, 2);

    QObject::connect(switchButton, SIGNAL(clicked()), renderWidgets, SLOT(SwitchRender()));
    QObject::connect(renderWidgets, SIGNAL(SwitchedRender(int)), renderOptionsWidget, SLOT(SwitchRender(int)));
    QObject::connect(renderOptionsWidget, SIGNAL(ShouldRayTrace()), renderWidgets, SLOT(RayTrace()));

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