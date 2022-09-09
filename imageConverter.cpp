#include <imageConverter.h>
#include <QPainter>
#include <QVector>

static const QSize resultSize(480, 480);

ImageConverter::ImageConverter()
{

}

// загрузка изображения с пути fileName в изображение image и realImage и помещение его в карту пикселей pic
void ImageConverter::loadImage(QString &fileName, QImage &image, QImage &realImage, QGraphicsPixmapItem* pic)
{
    filePath = fileName;
    image.load(fileName);
    realImage.load(fileName);
    image = image.scaled(resultSize, Qt::KeepAspectRatio);

    QImage fixedImage(resultSize, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&fixedImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(fixedImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(imagePos(image), image);
    painter.end();

    image = fixedImage;
    pic->setPixmap(QPixmap::fromImage(image));
}

// конвертация изображения в градации серого
void ImageConverter::toGray(QImage &imgRight, settings *sets)
{
    for (int i = 0; i < imgRight.width(); i++)
        for (int j = 0; j < imgRight.height(); j++)
        {
            QColor prevCol = imgRight.pixelColor(i, j);
            int col = sets->getGradientRed() * prevCol.red() + sets->getGradientGreen() * prevCol.green() +
                    sets->getGradientBlue() * prevCol.blue();
                imgRight.setPixelColor(i, j, QColor(col, col, col, 255));
        }
}

// формула для корретного расположения изображения в окне
QPoint ImageConverter::imagePos(const QImage &image) const
{
    return QPoint((resultSize.width() - image.width()) / 2,
                  (resultSize.height() - image.height()) / 2);
}

// функция для метода робертса, сканирующее окно 2x2
void ImageConverter::outline2x2(QImage &imgRight, settings* sets)
{
    for (int i = 0; i < imgRight.width() - 1; i++)
        for (int j = 0; j < imgRight.height() - 1; j++)
        {
            QColor l1 = imgRight.pixelColor(i, j);
            QColor l2 = imgRight.pixelColor(i + 1, j);
            QColor l3 = imgRight.pixelColor(i + 1, j + 1);
            QColor l4 = imgRight.pixelColor(i, j + 1);
            int gx = l3.red() - l1.red();
            int gy = l2.red() - l4.red();
            int g = sqrt(pow(gx, 2) + pow(gy, 2));

            if (sets->getNeedEdge() == true)
            {
                if (g > sets->getEdge())
                    imgRight.setPixelColor(i, j, QColor(0, 0, 0, 255));
                else
                    imgRight.setPixelColor(i, j, QColor(255, 255, 255, 255));
            }
            else
                imgRight.setPixelColor(i, j, QColor(g, g, g, 255));
        }
}

// функция для остальных методов, сканирующее окно 3x3
void ImageConverter::outline3x3(QImage &imgRight, settings *sets)
{
    QVector<QVector<int>*>* vec = new QVector<QVector<int>*>;
    for (int i = 1; i < imgRight.width() - 1; i++)
    {
        QVector<int>* tempVec = new QVector<int>;
        for(int j = 1; j < imgRight.height() - 1; j++)
        {
            QColor l0 = imgRight.pixelColor(i - 1, j - 1);
            QColor l1 = imgRight.pixelColor(i, j - 1);
            QColor l2 = imgRight.pixelColor(i + 1, j - 1);
            QColor l3 = imgRight.pixelColor(i + 1, j);
            QColor l4 = imgRight.pixelColor(i + 1, j + 1);
            QColor l5 = imgRight.pixelColor(i, j + 1);
            QColor l6 = imgRight.pixelColor(i - 1, j + 1);
            QColor l7 = imgRight.pixelColor(i - 1, j);
            QColor l8 = imgRight.pixelColor(i, j);

            int gx, gy;
            int g = 0;

            // метод кирша
            if (sets->getGradientMode() == 5)
            {
                QVector<int> col;
                col.append(l0.red());
                col.append(l1.red());
                col.append(l2.red());
                col.append(l3.red());
                col.append(l4.red());
                col.append(l5.red());
                col.append(l6.red());
                col.append(l7.red());
                int max = 0;
                for (int k = 1; k < 8; k++)
                {
                    int s = col[k] + col[(k + 1) % 8] + col[(k + 2) % 8];
                    int t = col[(k + 3) % 8] + col[(k + 4) % 8] + col[(k + 5) % 8] +
                            col[(k + 6) % 8] + col[(k + 7) % 8];
                    int f = abs(5 * s - 3 * t);
                    if (f > max)
                        max = f;
                }
                g = max;
            }
            // лапласиан
            else if (sets->getGradientMode() == 4)
            {
                g = 8 * l8.red() - (l0.red() + l1.red() + l2.red() + l3.red() +
                                    l4.red() + l5.red() + l6.red() + l7.red());
            }
            // собела
            else if (sets->getGradientMode() == 3)
            {
                gx = (l2.red() + 2 * l3.red() + l4.red()) - (l0.red() + l6.red() + 2* l7.red());
                gy = (l4.red() + 2 * l5.red() + l6.red()) - (l0.red() + 2 * l1.red() + l2.red());
                g = sqrt(pow(gx, 2) + pow(gy, 2));
            }
            // превитта
            else
            {
                gx = (l2.red() + l3.red() + l4.red()) - (l0.red() + l6.red() + l7.red());
                gy = (l4.red() + l5.red() + l6.red()) - (l0.red() + l1.red() + l2.red());
                g = sqrt(pow(gx, 2) + pow(gy, 2));
            }
            tempVec->append(g);
        }
        vec->append(tempVec);
    }

    for (int i = 1; i < imgRight.width() - 1; i++)
    {
        for(int j = 1; j < imgRight.height() - 1; j++)
        {
            if (sets->getNeedEdge() == true)
            {
                if (vec->at(i - 1)->at(j - 1) > sets->getEdge())
                    imgRight.setPixelColor(i, j, QColor(0, 0, 0, 255));
                else
                    imgRight.setPixelColor(i, j, QColor(255, 255, 255, 255));
            }
            else
                imgRight.setPixelColor(i, j, QColor(vec->at(i - 1)->at(j - 1), vec->at(i - 1)->at(j - 1), vec->at(i - 1)->at(j - 1), 255));
        }
    }
    delete vec;
}

// общая функция для выделения контуров
void ImageConverter::outlineImage(QImage imgLeft, QImage &imgRight, settings* sets)
{
    imgRight = imgLeft.convertToFormat(QImage::Format_ARGB32);
    toGray(imgRight, sets);

    if (sets->getGrayOnly() == false)
    {
        switch (sets->getGradientMode()) {
        case 1:
            outline2x2(imgRight, sets);
            break;
        default:
            outline3x3(imgRight, sets);
            break;
        }
    }

    QString split = "/";
    name = filePath.mid(filePath.lastIndexOf(split) + 1);
    name = name.split(".")[0];

    imgRight.save(sets->getPathRight() + name + "OUTLINE.png");
}
