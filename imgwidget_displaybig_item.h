#ifndef IMGWIDGETDISPLAYBIGITEM_H
#define IMGWIDGETDISPLAYBIGITEM_H

#include <QWidget>
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QEvent>
#include <QMouseEvent>
#include <QMouseEventTransition>
#include <QWheelEvent>
#include <QDebug>
class imgWidgetDisplayBigItem : public QGraphicsPixmapItem
{
public:
    imgWidgetDisplayBigItem();
    int scaleNum;
    void wheelEvent(QWheelEvent *e);
    void enterEvent(QEvent *e);
};

#endif // IMGWIDGETDISPLAYBIGITEM_H
