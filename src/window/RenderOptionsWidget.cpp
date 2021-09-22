//
// Render options widget class definition
//

#include <window/RenderOptionsWidget.h>

RenderOptionsWidget::RenderOptionsWidget(QWidget* parent, Transform& t, Camera& c) : 
    QWidget(parent), layout(nullptr), cameraGroupBox(nullptr), fovSpinBox(nullptr),
    windowAspectRatio(nullptr), aspectRatioWidthSpinBox(nullptr),
    aspectRatioHeightSpinBox(nullptr), rayTraceButton(nullptr), 
    transform(t), camera(c) {

    setMinimumWidth(200);

    // layout
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

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

    rayTraceButton = new QPushButton("RayTrace Image", this);
    QObject::connect(rayTraceButton, SIGNAL(pressed()), this, SLOT(PressedRayTrace()));
    rayTraceButton->setVisible(false);
    layout->addWidget(rayTraceButton);

    saveImageButton = new QPushButton("Save Image", this);
    QObject::connect(saveImageButton, SIGNAL(pressed()), this, SLOT(PressedSaveImage()));
    saveImageButton->setVisible(false);
    layout->addWidget(saveImageButton);
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

void RenderOptionsWidget::UpdateCamera() {
    std::cout << "Update Camera\n";
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