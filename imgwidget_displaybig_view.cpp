#include "imgwidget_displaybig_view.h"

imgWidgetDisplayBigView::imgWidgetDisplayBigView(QWidget *parent = 0):QGraphicsView(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setStyleSheet("background:transparent;border:0px");

    sameimgFlag = 0;
    scaleFlag = 0;
    currentfile = "";
    item = new imgWidgetDisplayBigItem;
    reciItem = new QGraphicsPixmapItem;
    this->setScene(&scene);
    item->setFlags(QGraphicsPixmapItem::ItemIsMovable);
    scene.addItem(item);
    scene.setSceneRect(0,0,800,600);
    this->show();
    thread = new imgWidgetDisplayThread;
    connect(thread,SIGNAL(sendThumb(QImage,QString)),this,SLOT(receiveFormThread(QImage,QString)));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(startThread()));
    timer->setSingleShot(true);
    keepSameTimer = new QTimer(this);
    connect(keepSameTimer, SIGNAL(timeout()), this, SLOT(startThread()));
    keepSameTimer->setSingleShot(true);
}

void imgWidgetDisplayBigView::receiveFormThread(QImage img2,QString recifilename){
    qDebug()<<"currentfile == fileNam"<<currentfile<<recifilename;
    if(currentfile==recifilename){
        img = img2;
        scaleFlag = 0;//重新加载图片，清零
        double tim1 = static_cast<double>(getTickCount());           
        item->setPixmap(QPixmap::fromImage(img));
        item->setPos((this->width()-img.width())/2.0,(this->height()-img.height())/2.0);
        tim1 = ((double)getTickCount()- tim1)/getTickFrequency();
        //qDebug()<<"QPixmap::receiveFormThread()"<<tim1;
        //qDebug()<<"";
        qDebug()<<"222222currentfile == fileNam"<<currentfile<<recifilename;
    }else{
        qDebug()<<"different img";
        thread->exit(0);
        keepSameTimer->start(1000);
    }
}

void imgWidgetDisplayBigView::receiveFromViewImg(QImage reciveImg, QSize size){

    if(!reciveImg.isNull()){
        float multiple = (size.width()*1.0/this->width()>size.height()*1.0/this->height()
                    ?size.width()*1.0/this->width():size.height()*1.0/this->height());
        if(multiple>1)
            reciveImg = reciveImg.scaled(size.width()/multiple,size.height()/multiple,Qt::KeepAspectRatio);
        else{
            reciveImg = reciveImg.scaled(size.width(),size.height(),Qt::KeepAspectRatio);
        }
        item->setPixmap(QPixmap::fromImage(reciveImg));
        item->setPos((this->width()-reciveImg.width())/2.0,(this->height()-reciveImg.height())/2.0);
    }

}

//加载大图
void imgWidgetDisplayBigView::receiveFromView(QString fileName){   
    currentfile = fileName;
    if(!currentfile.isEmpty()){
        qDebug()<<"receiveFromView"<<currentfile;
        //qDebug()<<"breaknum"<<thread->breakNum;
        //keepSameTimer->start(2000);//1000 = 1s
        thread->exit(0);
        thread->widWidth = width()*1.0;
        thread->widHeight = height()*1.0;
        thread->fileName =currentfile;
        timer->start(500);
    }
}

void imgWidgetDisplayBigView::startThread(){
     qDebug()<<"startThread";
     thread->start();
     //timer->stop();
}

void imgWidgetDisplayBigView::wheelEvent(QWheelEvent *e){
        scaleFlag = 1;//缩放了，就不能随窗口缩放而改变大小了
        int numDegrees = e->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离
        int numSteps = numDegrees / 15;//滚动的步数，*15就是鼠标滚动的角度
        if (e->orientation() == Qt::Vertical) {
           // qDebug()<<"垂直滚动 = "<<numSteps;//垂直滚动
            qDebug()<<"e.xy"<<e->x()<<e->y();
            item->scaleNum+=numSteps;
            //item->setTransformOriginPoint(e->x()+view->width()/2,e->y()+this->height()/2);
            item->setScale(item->scaleNum/10.0);
        } else {
         qDebug()<<"水平滚动 = ";      //水平滚动
        }
        e->accept();//接收该事件
}
