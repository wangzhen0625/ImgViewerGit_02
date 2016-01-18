#ifndef IMGWIDGETDISPLAYBIGVIEW_H
#define IMGWIDGETDISPLAYBIGVIEW_H
#include <QDebug>
#include <QPixmap>
#include <QTimeLine>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsScale>
#include <QFileInfoList>
#include <QGraphicsItemAnimation>
#include <QTimer>
#include <opencv.hpp>
#include "imgwidget_displaybig_item.h"
#include "imgwidget_display_thread.h"
using namespace cv;
class imgWidgetDisplayBigView : public QGraphicsView
{
    Q_OBJECT
public:
    imgWidgetDisplayBigView(QWidget *parent);
    QGraphicsScene scene;
    imgWidgetDisplayBigItem * item;
    QGraphicsPixmapItem * reciItem;
    QGraphicsItemAnimation *itemAni;

    imgWidgetDisplayThread *thread;

    int imgNum;
    int sameimgFlag;
    //int selectedNum;
    int scaleFlag;
    QImage img;
    QTimer *timer;
    QTimer *keepSameTimer;
    QString currentfile ;//当前选中的文件
   // QFileInfoList allImgs;//文件对象链表
public slots:
    void receiveFromView(QString fileName);
    void receiveFromViewImg(QImage reciveImg,QSize size);
    void receiveFormThread(QImage img2, QString recifilename);
    void startThread();
protected:
//    void keyPressEvent(QKeyEvent *keyEvent);
    void wheelEvent(QWheelEvent *e);
//    void resizeEvent(QResizeEvent *e);

};



#endif // IMGWIDGETDISPLAYBIGVIEW_H
