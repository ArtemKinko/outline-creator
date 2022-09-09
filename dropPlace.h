#ifndef DROPPLACE_H
#define DROPPLACE_H

#include <QGraphicsItem>
#include <QtWidgets>
#include <QtDebug>

class MainWindow;

class DropPlace: public QGraphicsItem
{
public:
    DropPlace(MainWindow* parent);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;

private:
    MainWindow* win;
};

class OutlinePlace: public DropPlace
{
public:
    OutlinePlace(MainWindow* parent): DropPlace(parent)
    {
        setAcceptDrops(false);
    }

};

#endif // DROPPLACE_H
