#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imageConverter.h"
#include "dropPlace.h"
#include "settings.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>

class DropPlace;

class GraphicsView : public QGraphicsView
{
public:
    using QGraphicsView::QGraphicsView;

protected:
    void resizeEvent(QResizeEvent *) override
    {
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setLeftText(QString str, QColor col);
    void setRightText(QString str, QColor col);
    void loadImage(QString path);
    void loadRightImage(QString path);
    QImage getLeftImage() {return leftImage;};
    QString getPath() {return path;};
    QString getSavePath() {return savePath;};
    QString getName() {return name;};
    void updateSettingsUi();

private:
    Ui::MainWindow *ui;             // форма
    QGraphicsScene* sceneLeft;      // левая графическая сцена
    QGraphicsScene* sceneRight;     // правая графическая сцена
    GraphicsView* viewLeft;         // левый контейнер для графики
    GraphicsView* viewRight;        // правый контейнер для графики
    QGraphicsTextItem* leftLabel;   // левый текст
    QGraphicsTextItem* rightLabel;  // правый текст
    QImage leftImage;               // левое изображение
    QImage realImage;               // копия левого изображения
    QImage rightImage;              // правое изображение

    QString path = "";              // путь к исходному файлу
    QString savePath = "";          // директория сохранения
    QString name;                   // название файла

    ImageConverter* converter;          // конвертер
    QGraphicsPixmapItem* leftPicPlace;  // левая карта пикселей
    QGraphicsPixmapItem* rightPicPlace; // правая карта пикселей

    settings* sets;                     // настройки

public slots:
    void openDirectory();
    void resetGrayKoef();
    void changedSlider();
    void checkedGray();
    void checkedEdge();
    void resetExtra();
    void changedSpin();
    void changedRadio();
};
#endif // MAINWINDOW_H
