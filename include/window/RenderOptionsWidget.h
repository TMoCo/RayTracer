//
// Render options widget class declaration
//

#ifndef RENDER_OPTIONS_WIDGETS_H
#define RENDER_OPTIONS_WIDGETS_H 1

#include <render/Camera.h>

#include <math/Transform.h>

#include <qt5/QtWidgets/QtWidgets>

class RenderOptionsWidget : public QWidget {
Q_OBJECT
private:
    QVBoxLayout* layout;

    // camera options
    QGroupBox* cameraGroupBox;
    QDoubleSpinBox* fovSpinBox;
    QCheckBox* windowAspectRatio;
    QDoubleSpinBox* aspectRatioWidthSpinBox;
    QDoubleSpinBox* aspectRatioHeightSpinBox;

    // model options
    // ...

    QPushButton* rayTraceButton; // show / disable on render type

    // ref to objects modified by UI
    Transform& transform;
    Camera& camera;

public:
    RenderOptionsWidget(QWidget* parent, Transform& t, Camera& c);
    ~RenderOptionsWidget();

public slots:
    void SwitchRender(int renderIndex);
    void UpdateCamera();
    void PressedRayTrace();

signals:
    void ShouldRayTrace();
};

#endif