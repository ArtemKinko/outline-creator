#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("C:/Users/Nika_Lis/OneDrive/OutlineCreator/OutlineCreator/icon.png"));
    setWindowTitle("OutlineCreator by Kinko Artem");

    sets = new settings;

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    converter = new ImageConverter();
    leftImage = sets->getImageLeft();
    rightImage = sets->getImageRight();

    sceneLeft = new QGraphicsScene();
    sceneRight = new QGraphicsScene();

    viewLeft = new GraphicsView(sceneLeft);
    viewLeft->setRenderHint(QPainter::Antialiasing);
    viewLeft->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    viewRight = new GraphicsView(sceneRight);
    viewRight->setRenderHint(QPainter::Antialiasing);
    viewRight->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    DropPlace* leftDrop = new DropPlace(this);
    OutlinePlace* rightDrop = new OutlinePlace(this);

    sceneLeft->addItem(leftDrop);
    sceneRight->addItem(rightDrop);

    leftPicPlace = new QGraphicsPixmapItem(QPixmap::fromImage(leftImage));
    leftPicPlace->setPos(-240, -240);
    sceneLeft->addItem(leftPicPlace);

    rightPicPlace = new QGraphicsPixmapItem(QPixmap::fromImage(rightImage));
    rightPicPlace->setPos(-240, -240);
    sceneRight->addItem(rightPicPlace);

    leftLabel = new QGraphicsTextItem();
    QFont font("Calibri", 12);
    leftLabel->setFont(font);
    sceneLeft->addItem(leftLabel);

    rightLabel = new QGraphicsTextItem();
    rightLabel->setFont(font);
    sceneRight->addItem(rightLabel);

    QColor colLeft(255, 100, 100);
    setLeftText("Загрузите изображение в область", colLeft);
    QColor colRight(50, 50, 50);
    setRightText("В этой области скоро будет контур изображения", colRight);

    ui->mainLay->addWidget(viewLeft);
    ui->mainLay->addWidget(viewRight);

    connect(ui->buttonOpenDir, &QPushButton::clicked, this, &MainWindow::openDirectory);
    connect(ui->buttonReset, &QPushButton::clicked, this, &MainWindow::resetGrayKoef);
    connect(ui->sliderRed, &QSlider::sliderReleased, this, &MainWindow::changedSlider);
    connect(ui->sliderGreen, &QSlider::sliderReleased, this, &MainWindow::changedSlider);
    connect(ui->sliderBlue, &QSlider::sliderReleased, this, &MainWindow::changedSlider);
    connect(ui->checkGrayOnly, &QCheckBox::toggled, this, &MainWindow::checkedGray);
    connect(ui->checkEdge, &QCheckBox::toggled, this, &MainWindow::checkedEdge);
    connect(ui->buttonResetExtra, &QPushButton::clicked, this, &MainWindow::resetExtra);
    connect(ui->spinEdge, &QSpinBox::valueChanged, this, &MainWindow::changedSpin);
    connect(ui->radioButton_1, &QRadioButton::released, this, &MainWindow::changedRadio);
    connect(ui->radioButton_2, &QRadioButton::released, this, &MainWindow::changedRadio);
    connect(ui->radioButton_3, &QRadioButton::released, this, &MainWindow::changedRadio);
    connect(ui->radioButton_4, &QRadioButton::released, this, &MainWindow::changedRadio);
    connect(ui->radioButton_5, &QRadioButton::released, this, &MainWindow::changedRadio);

    updateSettingsUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// функция для установки текста в левом окне
void MainWindow::setLeftText(QString str, QColor col)
{
    int len = str.length();
    leftLabel->setPlainText(str);
    leftLabel->setPos(-len * 3.85, 260);
    leftLabel->setDefaultTextColor(col);
}

// функция для установки текста в правом окне
void MainWindow::setRightText(QString str, QColor col)
{
    int len = str.length();
    rightLabel->setPlainText(str);
    rightLabel->setPos(-len * 3.65, 260);
    rightLabel->setDefaultTextColor(col);
}

// функция для загрузки исходного изображения из пути path
void MainWindow::loadImage(QString path)
{
    QString split = "/";
    QString split2 = ".";
    name = path.mid(path.lastIndexOf(split) + 1);
    name = name.split(split2)[0];
    qDebug() << name;

    converter->loadImage(path, leftImage, realImage, leftPicPlace);

    sets->setImageLeft(leftImage);
    sets->print();
    converter->outlineImage(leftImage, realImage, sets);
}

// загрузка правого изображения из пути path
void MainWindow::loadRightImage(QString path)
{
    converter->loadImage(path, rightImage, realImage, rightPicPlace);
    sets->setImageRight(rightImage);
    sets->print();
}

// функция, обновляющая ui представление настроек
void MainWindow::updateSettingsUi()
{
    if (sets->getPathRight() != "")
        ui->linePath->setText(sets->getPathRight());
    ui->checkEdge->setChecked(sets->getNeedEdge());
    ui->spinEdge->setValue(sets->getEdge());
    ui->checkGrayOnly->setChecked(sets->getGrayOnly());

    switch(sets->getGradientMode())
    {
    case 1:
        ui->radioButton_1->setChecked(true);
        break;
    case 2:
        ui->radioButton_2->setChecked(true);
        break;
    case 3:
        ui->radioButton_3->setChecked(true);
        break;
    case 4:
        ui->radioButton_4->setChecked(true);
        break;
    case 5:
        ui->radioButton_5->setChecked(true);
        break;
    }

    ui->sliderRed->setValue(int(sets->getGradientRed() * 100));
    ui->sliderGreen->setValue(int(sets->getGradientGreen() * 100));
    ui->sliderBlue->setValue(100 - ui->sliderRed->value() - ui->sliderGreen->value());

    if (sets->getGrayOnly() == true)
    {
        ui->radioButton_1->setEnabled(false);
        ui->radioButton_2->setEnabled(false);
        ui->radioButton_3->setEnabled(false);
        ui->radioButton_4->setEnabled(false);
        ui->radioButton_5->setEnabled(false);
    }

    savePath = sets->getPathRight();
}

// слот нажатия на кнопку "..."
void MainWindow::openDirectory()
{
    QString temp = QFileDialog::getExistingDirectory(this, "Выберите папку для сохранения изображений") + "/";;
    if (temp != "/")
    {
        savePath = temp;
        sets->setPathRight(temp);
        ui->linePath->setText(savePath);
        sets->print();
    }
}

// слот нажатия на кнопку "по умолчинию" для коэффициентов серого
void MainWindow::resetGrayKoef()
{
    ui->sliderRed->setSliderPosition(30);
    ui->sliderGreen->setSliderPosition(59);
    ui->sliderBlue->setSliderPosition(11);

    sets->setGradientRed(0.30);
    sets->setGradientGreen(0.59);
    sets->setGradientBlue(0.11);
    sets->print();
}

// слот обработки изменения положения слайдеров
void MainWindow::changedSlider()
{
    int r = ui->sliderRed->sliderPosition();
    int g = ui->sliderGreen->sliderPosition();
    int b = ui->sliderBlue->sliderPosition();
    int dx = 100 - r - g - b;
    if (dx == 0)
        return;

    qDebug() << dx << "|" << r << g << b;

    int tempr = int(double(r) / (double(r) + double(g) + double(b)) * 100);
    int tempg = int(double(g) / (double(r) + double(g) + double(b)) * 100);
    int tempb = 100 - tempr - tempg;

    ui->sliderRed->setSliderPosition(tempr);
    ui->sliderGreen->setSliderPosition(tempg);
    ui->sliderBlue->setSliderPosition(tempb);

    sets->setGradientRed((double(tempr) / 100.0));
    sets->setGradientGreen((double(tempg) / 100.0));
    sets->setGradientBlue((double(tempb) / 100.0));

    qDebug() << ui->sliderRed->sliderPosition() << ui->sliderGreen->sliderPosition() << ui->sliderBlue->sliderPosition();
    qDebug() << ui->sliderRed->sliderPosition() + ui->sliderGreen->sliderPosition() + ui->sliderBlue->sliderPosition();
    sets->print();
}

// слот для обработки "только градации серого"
void MainWindow::checkedGray()
{
    if (ui->checkGrayOnly->isChecked())
    {
        ui->checkEdge->setEnabled(false);
        ui->spinEdge->setEnabled(false);
        ui->radioButton_1->setEnabled(false);
        ui->radioButton_2->setEnabled(false);
        ui->radioButton_3->setEnabled(false);
        ui->radioButton_4->setEnabled(false);
        ui->radioButton_5->setEnabled(false);
    }
    else
    {
        ui->radioButton_1->setEnabled(true);
        ui->radioButton_2->setEnabled(true);
        ui->radioButton_3->setEnabled(true);
        ui->radioButton_4->setEnabled(true);
        ui->radioButton_5->setEnabled(true);
        ui->checkEdge->setEnabled(true);
        if (ui->checkEdge->isChecked())
            ui->spinEdge->setEnabled(true);
    }
    sets->setGrayOnly(ui->checkGrayOnly->isChecked());
    sets->print();
}

// слот для обработки "сравнивать с пороговым"
void MainWindow::checkedEdge()
{
    ui->spinEdge->setEnabled(ui->checkEdge->isChecked());
    sets->setNeedEdge(ui->checkEdge->isChecked());
    sets->print();
}

// слот для кнопки "по умолчанию" для дополнительных параметров
void MainWindow::resetExtra()
{
    ui->checkEdge->setEnabled(true);
    ui->checkEdge->setChecked(true);
    ui->checkGrayOnly->setChecked(false);
    ui->spinEdge->setEnabled(true);
    ui->spinEdge->setValue(50);

    sets->setNeedEdge(true);
    sets->setEdge(50);
    sets->setGrayOnly(false);
    sets->print();
}

// слот для обработки изменений спина границы
void MainWindow::changedSpin()
{
    sets->setEdge(ui->spinEdge->value());
    sets->print();
}

// слот для обработки изменений режима градиента
void MainWindow::changedRadio()
{
    sets->setGradientMode((sender()->objectName().right(1).toInt()));
    sets->print();
}
