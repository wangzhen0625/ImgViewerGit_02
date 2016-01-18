#include "widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(814,512);
    w_imgWidget = new imgWidget(this);
    slideflag = 0;
    slideTimer = new QTimer(this);
    connect(slideTimer, SIGNAL(timeout()), w_imgWidget->thumb_view, SLOT(playSlide()));
    this->show();

}

void Widget::contextMenuEvent(QContextMenuEvent *event){
    QMenu *pop_menu = new QMenu();
    QAction *act0 = new QAction(QIcon("../l1.png"), tr("&打开文件..."), this);
    QAction *act1 = new QAction(QIcon("../l1.png"), tr("&复制..."), this);
    QAction *act2 = new QAction(QIcon("../l1.png"), tr("&删除..."), this);
    QAction *act3 = new QAction(QIcon("../l1.png"), tr("&另存为..."), this);
    QAction *act4 = new QAction(QIcon("../l1.png"), tr("&关闭软件..."), this);
    QAction *act5 = new QAction(QIcon("../l1.png"), tr("&刷新..."), this);
    QAction *act6 = new QAction(QIcon("../l1.png"), tr("&设为壁纸..."), this);
    QAction *act7 = new QAction(QIcon("../l1.png"), tr("&设置..."), this);
    QAction *act8 = new QAction(QIcon("../l1.png"), tr("&幻灯片..."), this);
    QAction *act9 = new QAction(QIcon("../l1.png"), tr("&out幻灯片..."), this);
    pop_menu->clear();
    pop_menu->addAction(act0);
    pop_menu->addAction(act1);
    pop_menu->addAction(act2);
    pop_menu->addAction(act3);
    pop_menu->addAction(act4);
    pop_menu->addAction(act5);
    pop_menu->addAction(act6);
    pop_menu->addAction(act7);
    pop_menu->addAction(act8);
    pop_menu->addAction(act9);
    act0->setStatusTip(tr("Open an existing file"));
    connect(act0, SIGNAL(triggered()), w_imgWidget, SLOT(openFile()));
    connect(act1, SIGNAL(triggered()), w_imgWidget->thumb_view, SLOT(copyFile()));
    connect(act2, SIGNAL(triggered()), w_imgWidget->thumb_view, SLOT(removeFile()));
    connect(act3, SIGNAL(triggered()), w_imgWidget->thumb_view, SLOT(saveAs()));
    connect(act4, SIGNAL(triggered()), this, SLOT(close()));
    connect(act5, SIGNAL(triggered()), w_imgWidget, SLOT(loadAllFile()));
    connect(act6, SIGNAL(triggered()), w_imgWidget->thumb_view, SLOT(setAsWallpaper()));
    connect(act8, SIGNAL(triggered()), this, SLOT(playSlide()));
    connect(act9, SIGNAL(triggered()), this, SLOT(stopSlide()));

    //菜单出现的位置为当前鼠标的位置
   pop_menu->exec(QCursor::pos());
   event->accept();
}

//1.键盘快捷键
void Widget::keyPressEvent(QKeyEvent *keyEvent)
{
 int keyValue = keyEvent->key();
 switch (keyValue) {
 case Qt::Key_Q:
     qDebug()<<"Key_Q = "<<keyValue;
     break;
 case Qt::Key_O:
     qDebug()<<"Key_O = "<<keyValue;
     w_imgWidget->openFile();
     break;
 case Qt::Key_H:
     qDebug()<<"Key_O = "<<keyValue;
     break;
 case Qt::Key_I:
     break;
 default:
     break;
 }
}

void Widget::playSlide(){
    if(slideflag == 0){
        slideflag = 1;
        this->showMaximized();
        this->showFullScreen();
        slideTimer->start(1000);
    }
}

void Widget::stopSlide(){
    this->showNormal();
    slideTimer->stop();
    slideflag = 0;
}

Widget::~Widget()
{

}
void Widget::resizeEvent(QResizeEvent *e){//
    w_imgWidget->changeSize(this->width(),this->height());
}
