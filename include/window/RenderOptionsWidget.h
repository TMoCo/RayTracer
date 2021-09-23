//
// Render options widget class declaration
//

#ifndef RENDER_OPTIONS_WIDGETS_H
#define RENDER_OPTIONS_WIDGETS_H 1

#include <render/Camera.h>

#include <math/Transform.h>

#include <window/VectorWidget.h>

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
    VectorWidget* positionWidget;
    // VectorWidget* rotationWidget;
    // ...

    QPushButton* rayTraceButton; // show / disable on render type
    QPushButton* saveImageButton;

public:
    RenderOptionsWidget(QWidget* parent, Transform* transform, Camera* camera);
    ~RenderOptionsWidget();

public slots:
    void SwitchRender(int renderIndex);   
    void UpdateParameters(); 
    void PressedRayTrace();
    void PressedSaveImage();

signals:
    void ShouldUpdateGl();
    void ShouldRayTrace();
    void ShouldSaveImage(QString name);
};

#endif