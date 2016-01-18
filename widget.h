#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QDebug>
#include "imgwidget.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    void contextMenuEvent(QContextMenuEvent *event);//鼠标右键快捷键
    void keyPressEvent(QKeyEvent *keyEvent);//键盘按键快捷键
    int slideflag;
    QTimer *slideTimer;
    ~Widget();

    imgWidget * w_imgWidget;\
public slots:
    void playSlide();
    void stopSlide();
protected:
    void resizeEvent(QResizeEvent *e);
};

#endif // WIDGET_H
