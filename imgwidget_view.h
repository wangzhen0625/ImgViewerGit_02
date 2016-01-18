#ifndef IMGWIDGETVIEW_H
#define IMGWIDGETVIEW_H

#include <QApplication>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>

#include <QLabel>
#include <QDir>
#include <QFileDialog>
#include <QMouseEvent>
#include <QTextItem>
#include <QTextDocument>
#include <QRadialGradient>
#include <QFileInfo>
#include <QFile>
#include <QTimeLine>
#include <QTimer>
#include <QClipboard>
#include <QGraphicsItemAnimation>
#include "imgwidget_thumbthread.h"
#include <QDebug>
#include <QDataStream>
#include <QProcess>
#include <QMessageBox>
#include <QMap>
class imgwidgetview : public QGraphicsView
{
    Q_OBJECT
public:   
    imgwidgetview(QWidget *parent=0);

    int wheelFlag;
    int imgNum;
    int widwidth;
    int widheight;
    QString currentfile ;//当前选中的文件
    QFileInfoList allImgs;//文件对象链表

    QGraphicsScene scene;
    QMap<int,QImage> all_Qimg;
    QMap<int,QSize> all_size;
    QList<QGraphicsPixmapItem *> allPixItem;
    QGraphicsPixmapItem *midPixItemFlag;
    QList<QGraphicsPixmapItem *> midPixItems;
    QList<QGraphicsItemAnimation*> allAnimation;
    imgwidgetThumbThread * ThumbThread;
    imgwidgetThumbThread * saveAsThread;
    int selectedNum;
    int oldSelectNum;

    void setItem();//读取当前文件所有图片
    void setItemPos(int i);
    void movItemPos(int i);
    void changeSelectNum();
public slots:
    void itemAddImg(QImage fullImg, QImage halfImg, QSize size, int pos);
    void removeFile();
    void copyFile();
    void saveAs();
    void setAsWallpaper();
    void preImg(int num = 1);
    void nextImg(int num = 1);
    void timelineFinished();
    void wheelChangeImg();
    void playSlide();

protected:
    //void mouseReleaseEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *e);
signals:
    void sendToDisplay(QString currentfile);
    void sendToDisplayImg(QImage img,QSize size);
};

#endif // IMGWIDGETVIEW_H
