#include "settings.h"
#include <QDebug>
#include <QProcess>

settings::settings(): QSettings("OutlineCreator", "Artem Kinko")
{
    readSettings();
}

void settings::setNeedEdge(bool temp)
{
    needEdge = temp;
    setValue("needEdge", needEdge);
}

void settings::setEdge(int temp)
{
    edge = temp;
    setValue("edge", edge);
}

void settings::setGrayOnly(bool temp)
{
    grayOnly = temp;
    setValue("grayOnly", grayOnly);
}

void settings::setGradientMode(unsigned char temp)
{
    gradientMode = temp;
    setValue("gradientMode", gradientMode);
}

void settings::setGradientRed(double temp)
{
    gradientRed = temp;
    setValue("gradientRed", gradientRed);
}

void settings::setGradientGreen(double temp)
{
    gradientGreen = temp;
    setValue("gradientGreen", gradientGreen);
}

void settings::setGradientBlue(double temp)
{
    gradientBlue = temp;
    setValue("gradientBlue", gradientBlue);
}

void settings::setPathLeft(QString temp)
{
    pathLeft = temp;
    setValue("pathLeft", pathLeft);
}

void settings::setPathRight(QString temp)
{
    pathRight = temp;
    setValue("pathRight", pathRight);
}

void settings::setImageLeft(QImage temp)
{
    imageLeft = temp;
    setValue("imageLeft", imageLeft);
}

void settings::setImageRight(QImage temp)
{
    imageRight = temp;
    setValue("imageRight", imageRight);
}

bool settings::getNeedEdge()
{
    return needEdge;
}

int settings::getEdge()
{
    return edge;
}

bool settings::getGrayOnly()
{
    return grayOnly;
}

unsigned char settings::getGradientMode()
{
    return gradientMode;
}

double settings::getGradientRed()
{
    return gradientRed;
}

double settings::getGradientGreen()
{
    return gradientGreen;
}

double settings::getGradientBlue()
{
    return gradientBlue;
}

QString settings::getPathLeft()
{
    return pathLeft;
}

QString settings::getPathRight()
{
    return pathRight;
}

QImage settings::getImageLeft()
{
    return imageLeft;
}

QImage settings::getImageRight()
{
    return imageRight;
}

void settings::print()
{
    qDebug() << "\n\n\n\n\n\n";
    qDebug() << "----------";
    qDebug() << "Настройки:";
    qDebug() << "Коэффициент красного: " << gradientRed;
    qDebug() << "Коэффициент зеленого: " << gradientGreen;
    qDebug() << "Коэффициент голубого: " << gradientBlue;
    qDebug() << "Путь к левой картинке: " << pathLeft;
    qDebug() << "Путь к папке сохранения: " << pathRight;
    qDebug() << "Режим градиента: " << gradientMode;
    qDebug() << "Только серое: " << grayOnly;
    qDebug() << "Сравнивать с пороговым: " << needEdge;
    qDebug() << "Значение порога: " << edge;
    qDebug() << "Левая картинка: " << imageLeft;
    qDebug() << "Правая картинка: " << imageRight;

}

// считывающая функция, загружающая настройки
void settings::readSettings()
{
    // qDebug() << value("loading").toInt();
    if (value("ready").toInt() == 0)
    {
        setValue("ready", 1);
        setValue("needEdge", true);
        setValue("edge", 50);
        setValue("grayOnly", false);
        setValue("gradientMode", 1);
        setValue("gradientRed", 0.3);
        setValue("gradientGreen", 0.59);
        setValue("gradientBlue", 0.11);
        setValue("pathLeft", "");
        setValue("pathRight", "");
        setValue("imageLeft", QImage());
        setValue("imageRight", QImage());
    }
    else
    {
        needEdge = value("needEdge").toBool();
        edge = value("edge").toInt();
        grayOnly = value("grayOnly").toBool();
        gradientMode = value("gradientMode").toInt();
        gradientRed = value("gradientRed").toDouble();
        gradientGreen = value("gradientGreen").toDouble();
        gradientBlue = value("gradientBlue").toDouble();
        pathRight = value("pathRight").toString();
        pathLeft = value("pathLeft").toString();
        imageLeft = qvariant_cast<QImage>(value("imageLeft"));
        imageRight = qvariant_cast<QImage>(value("imageRight"));
    }
}
