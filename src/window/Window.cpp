//
// Window class declaration
//

#include <window/Window.h>

Window::Window(const Model& model, const char* name) : QWidget(nullptr), 
    layout(nullptr), renderWidgets(nullptr), switchButton(nullptr) {
    setWindowTitle(QString(name));

    layout = new QVBoxLayout(this);

    renderWidgets = new RenderWidgets(model, this);
    switchButton = new QPushButton("Switch Render", this);

    layout->addWidget(renderWidgets);
    layout->addWidget(switchButton);

    QObject::connect(switchButton, SIGNAL(clicked()), renderWidgets, SLOT(SwitchWidget()));
}

Window::~Window() {
    delete renderWidgets;
    delete switchButton;
    delete layout;
}