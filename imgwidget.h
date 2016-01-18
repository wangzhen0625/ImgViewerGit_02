#ifndef IMGWIDGET_H
#define IMGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

#include <QTextItem>
#include <QTextDocument>
#include <QRadialGradient>

#include <QImage>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "imgwidget_view.h"
#include "imgwidget_displaybig_view.h"
class imgWidget:public QWidget
{
    Q_OBJECT
public:
    imgWidget(QWidget * parent);

    int imgNum;
    //QString currentFile ;//当前选中的文件
    QFileInfoList allImgs;//图像对象链表
    imgwidgetview * thumb_view;
    //大的图片显示
    imgWidgetDisplayBigView * displayView;


public slots:
    void openFile();//获取当前选中的文件currentFile，传给view
    void loadAllFile();//获取当前选中的文件currentFile，传给view
    void changeSize(int width,int height);
protected:
    void resizeEvent(QResizeEvent *e);


};

#endif // IMGWIDGET_H
