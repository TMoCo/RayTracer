//
// Window class declaration
//

#include <window/Window.h>

Window::Window(Model* model, const char* name) : QWidget(nullptr), 
    layout(nullptr), renderWidgets(nullptr), switchButton(nullptr) {
    setWindowTitle(QString(name));

    layout = new QGridLayout(this);

    renderWidgets = new RenderWidgets(model, this);
    switchButton = new QPushButton("Switch Render", this);

    layout->addWidget(renderWidgets, 0, 0);
    layout->addWidget(switchButton, 1, 0, 1, 2);

    QObject::connect(switchButton, SIGNAL(clicked()), renderWidgets, SLOT(SwitchWidget()));
}

Window::~Window() {
    delete renderWidgets;
    delete switchButton;
    delete layout;
}