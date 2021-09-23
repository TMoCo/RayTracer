//
// Render options widget class definition
//

#include <window/RenderOptionsWidget.h>

RenderOptionsWidget::RenderOptionsWidget(QWidget* parent, Transform* transform, 
    Camera* camera) : QWidget(parent) {

    setMinimumWidth(200);

    // layout
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // camera editing
    QLabel* cameraOptionsLabel = new QLabel("Camera options", this);
    cameraOptionsLabel->setStyleSheet( "QLabel { font-weight: bold; }" );
    layout->addWidget(cameraOptionsLabel);

    cameraGroupBox = new QGroupBox(this);
    cameraGroupBox->setStyleSheet(
        "QGroupBox {  border: 1px solid lightgray; font-weight: bold;  } ");
    QVBoxLayout* groupBoxLayout = new QVBoxLayout(cameraGroupBox);
    
    QLabel* fovLabel = new QLabel("FOV", this);
    groupBoxLayout->addWidget(fovLabel);
    
    fovSpinBox = new QDoubleSpinBox(nullptr);
    fovSpinBox->setSuffix("°");
    fovSpinBox->setRange(0.0, 180.0);
    groupBoxLayout->addWidget(fovSpinBox);

    QLabel* aspectRatioLabel = new QLabel("Aspect Ratio", this);
    groupBoxLayout->addWidget(aspectRatioLabel);
    windowAspectRatio = new QCheckBox("Use window dimensions", this);
    groupBoxLayout->addWidget(windowAspectRatio);

    aspectRatioWidthSpinBox = new QDoubleSpinBox(this);
    groupBoxLayout->addWidget(aspectRatioWidthSpinBox);
    aspectRatioHeightSpinBox = new QDoubleSpinBox(this);
    groupBoxLayout->addWidget(aspectRatioHeightSpinBox);

    layout->addWidget(cameraGroupBox);

    // transform editing
    positionWidget = new VectorWidget(&transform->position, -10.0f, 10.0f, this);
    layout->addWidget(positionWidget);    
    QObject::connect(
        positionWidget, SIGNAL(ValueChanged()), 
        this, SLOT(UpdateParameters()));

    // raytracing
    rayTraceButton = new QPushButton("RayTrace Image", this);
    rayTraceButton->setVisible(false);
    layout->addWidget(rayTraceButton);
    QObject::connect(
        rayTraceButton, SIGNAL(pressed()), 
        this, SLOT(PressedRayTrace()));

    saveImageButton = new QPushButton("Save Image", this);
    saveImageButton->setVisible(false);
    layout->addWidget(saveImageButton);
    QObject::connect(
        saveImageButton, SIGNAL(pressed()), 
        this, SLOT(PressedSaveImage()));
}

RenderOptionsWidget::~RenderOptionsWidget() {
    delete rayTraceButton;
    
    delete aspectRatioHeightSpinBox;
    delete aspectRatioWidthSpinBox;
    delete windowAspectRatio;
    delete fovSpinBox;

    delete cameraGroupBox;

    delete layout;     
}

void RenderOptionsWidget::SwitchRender(int renderIndex) {
    if (renderIndex == 1) {
        rayTraceButton->setVisible(true);
        saveImageButton->setVisible(true);
    }
    else {
        rayTraceButton->setVisible(false);
        saveImageButton->setVisible(false);
    }
}

void RenderOptionsWidget::UpdateParameters() {
    // called whenever an option in UI has been changed
    emit ShouldUpdateGl();
}

void RenderOptionsWidget::PressedRayTrace() {
    emit ShouldRayTrace(); // emit signal
}

void RenderOptionsWidget::PressedSaveImage() {
    bool ok;
    QString text = QInputDialog::getText(this, "Save Image", "Enter file name:", QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) 
        emit ShouldSaveImage(text);
}