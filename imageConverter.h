#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H
#include <settings.h>

#include <QString>
#include <QImage>
#include <QGraphicsPixmapItem>

class ImageConverter
{
public:
    ImageConverter();
    void loadImage(QString &fileName, QImage &image, QImage& realImage, QGraphicsPixmapItem* pic);
    void saveImage(QString savePath);
    void saveOutline(QString savePath);
    void toGray(QImage &imgRight, settings* sets);
    void outlineImage(QImage imgLeft, QImage &imgRight, settings* sets);

private:
    QPoint imagePos(const QImage &image) const;
    const unsigned char* getRGBLeft(int x, int y);
    void outline2x2(QImage &imgRight, settings* sets);
    void outline3x3(QImage &imgRight, settings* sets);

    QString filePath;   // путь к исходному файлу
    QString name;       // имя исходного файла
};

#endif // IMAGECONVERTER_H
