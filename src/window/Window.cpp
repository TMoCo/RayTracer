//
// Window class declaration
//

#include <window/Window.h>

Window::Window(const Model& model, const char* name) : QWidget(nullptr), glWidget(nullptr) {
    setWindowTitle(QString(name));

    glWidget = new OpenGLWidget(model, this);

    layout = new QVBoxLayout(this);

    timer = new QTimer(this);

    timer->setInterval(16); // 16 ms

    //QObject::connect(timer, SIGNAL(timeout()), glWidget, SLOT(update()));

    timer->start();

    layout->addWidget(glWidget);
}