#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QImage>
#include <QSettings>

class settings : public QSettings
{
public:
    settings();
    void setNeedEdge(bool temp);
    void setEdge(int temp);
    void setGrayOnly(bool temp);
    void setGradientMode(unsigned char temp);
    void setGradientRed(double temp);
    void setGradientGreen(double temp);
    void setGradientBlue(double temp);
    void setPathLeft(QString temp);
    void setPathRight(QString temp);
    void setImageLeft(QImage temp);
    void setImageRight(QImage temp);

    bool getNeedEdge();
    int getEdge();
    bool getGrayOnly();
    unsigned char getGradientMode();
    double getGradientRed();
    double getGradientGreen();
    double getGradientBlue();
    QString getPathLeft();
    QString getPathRight();
    QImage getImageLeft();
    QImage getImageRight();

    void print();
    void readSettings();

private:
    bool needEdge = true;               // сравнивать с пороговым
    int edge = 50;                      // порог
    bool grayOnly = false;              // только градации серого
    unsigned char gradientMode = 1;     // выбранный градиентный оператор
    double gradientRed = 0.3;           // коэффициент красного для градаций серого
    double gradientGreen = 0.59;        // коэффициент зеленого
    double gradientBlue = 0.11;         // коэффициент синего
    QString pathLeft = "";              // путь к левому изображению
    QString pathRight = "";             // путь к правому изображению
    QImage imageLeft;                   // левое изображение
    QImage imageRight;                  // правое изображение
};

#endif // SETTINGS_H
