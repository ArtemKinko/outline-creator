#include <dropPlace.h>

#include <QCursor>
#include <QPainter>

#include <mainwindow.h>

DropPlace::DropPlace(MainWindow* parent)
{
    setAcceptDrops(true);
    win = parent;
}

// метод отрисовки границ полей для главной страницы
void DropPlace::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(Qt::darkGray);
    painter->setPen(QPen(Qt::black, 1));

    static const QPointF pointsLU[6] = {
        QPointF(-250, -250),
        QPointF(-250, -200),
        QPointF(-240, -200),
        QPointF(-240, -240),
        QPointF(-200, -240),
        QPointF(-200, -250)
    };

    static const QPointF pointsRD[6] = {
        QPointF(250, 250),
        QPointF(250, 200),
        QPointF(240, 200),
        QPointF(240, 240),
        QPointF(200, 240),
        QPointF(200, 250)
    };

    static const QPointF pointsLD[6] = {
        QPointF(-250, 250),
        QPointF(-250, 200),
        QPointF(-240, 200),
        QPointF(-240, 240),
        QPointF(-200, 240),
        QPointF(-200, 250)
    };

    static const QPointF pointsRU[6] = {
        QPointF(250, -250),
        QPointF(250, -200),
        QPointF(240, -200),
        QPointF(240, -240),
        QPointF(200, -240),
        QPointF(200, -250)
    };

    painter->drawConvexPolygon(pointsLU, 6);
    painter->drawConvexPolygon(pointsRD, 6);
    painter->drawConvexPolygon(pointsLD, 6);
    painter->drawConvexPolygon(pointsRU, 6);
}

// ивент, возникающий при удержании дропа в зоне
void DropPlace::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    Q_UNUSED(event);
    win->setLeftText("Отпустите кнопку, чтобы загрузить изображение", QColor(100, 100, 100));
}

// ивент, возникающий при выходе из зоны при удержании дропа
void DropPlace::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
    Q_UNUSED(event);
    win->setLeftText("Загрузите изображение в область", QColor(255, 100, 100));
}

// ивент, вызывающийся при отпускании дропа в зоне
void DropPlace::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QUrl path = event->mimeData()->urls().at(0);
    QString str;
    str = path.toString();
    if (str.right(4) == ".png" || str.right(4) == ".jpg")
    {
        qDebug() << "OK";
        str = str.mid(8);
        win->loadImage(str);
        win->setLeftText("Успешно загружено!", QColor(50, 150, 50));
        qDebug() << "BRUH" << win->getPath() + win->getName() + "OUTLINE.png";
        win->loadRightImage(win->getSavePath() + win->getName() + "OUTLINE.png");
        win->setRightText((win->getSavePath() + win->getName() + "OUTLINE.png"), QColor(50, 150, 50));
    }
    else
        win->setLeftText("Загружаемый файл должен иметь расширение .png или .jpg!", QColor(150, 50, 50));
}

// определение зоны дропа в окне
QRectF DropPlace::boundingRect() const
{
    return QRectF(-250, -250, 500, 500);
}
