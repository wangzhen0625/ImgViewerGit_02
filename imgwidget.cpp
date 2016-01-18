#include "imgwidget.h"

imgWidget::imgWidget(QWidget *parent):QWidget(parent)
{
    //缩略图
    displayView = new imgWidgetDisplayBigView(this);
    displayView->show();
    displayView->setGeometry(0,0,this->width(),this->height());
    thumb_view = new imgwidgetview(this);
    thumb_view->setGeometry(0,this->height()-81,this->width(),48);
    thumb_view->show();

    connect(thumb_view,SIGNAL(sendToDisplay(QString))
            ,displayView,SLOT(receiveFromView(QString)));
    connect(thumb_view,SIGNAL(sendToDisplayImg(QImage,QSize))
            ,displayView,SLOT(receiveFromViewImg(QImage,QSize)));

}

/****************************
1.
****************************/
void imgWidget::openFile(){
    //qDebug()<<"start openFile";
    //设置停止线程标志位，让线程停止
    QString opendir = QDir::homePath();//指定默认打开的路径
    thumb_view->currentfile = QFileDialog::getOpenFileName(this,"Open file",opendir,//从默认打开的路径开始查找选择文件
            "Images (*.bmp *.gif *.tif *.jpg *.jpeg *.jpeg2000 *.png *.pbm *.pgm *.ppm *.xbm *.xpm *.tiff)");
    loadAllFile();
}

/****************************
2.
****************************/
void imgWidget::loadAllFile(){
    if((QFileInfo(thumb_view->currentfile).isFile())&&(thumb_view->currentfile.trimmed()!=""))//判断是不是文件并且没有取消选择文件
    {
        QString  currentdir;//获取这个文件的路径
        currentdir = QFileInfo(thumb_view->currentfile).absoluteDir().absolutePath();//获取最终选中的文件的路径
        //qDebug() << "获取这个文件的路径:" << currentdir;//获取这个文件的路径
        QDir *dir = new QDir(currentdir);//把获取的路径赋值给文件夹变量进行数据操作
        //qDebug()<<"currentFile"<<thumb_view->currentfile;
        //过滤器
        QStringList flt;
        flt << "*.bmp" << "*.gif" << "*.tif" << "*.jpg" << "*.jpeg" << "*.jpeg2000" << "*.png" << "*.pbm" << "*.pgm" << "*.ppm" << "*.xbm" << "*.xpm" << "*.tiff";
        dir->setNameFilters(flt);

        allImgs = dir->entryInfoList(QDir::NoFilter,QDir::IgnoreCase);//返回文件和目录的对象
        imgNum = allImgs.length();//但前链表的长度
        //缩略图的实现
        thumb_view->allImgs = allImgs;
        thumb_view->imgNum = imgNum;
        thumb_view->widwidth = this->width();
        thumb_view->widheight = this->height();
        thumb_view->setItem();
    }else{
        QMessageBox message(QMessageBox::NoIcon, "提示", "请先选择图片文件!");
        message.setIconPixmap(QPixmap("../2.png"));
        message.exec();
    }

}

void imgWidget::changeSize(int width, int height){//widget send to this
   this->resize(width,height);
}\

//他的主窗口改变大小，调用主窗口的resizeevent，不调用这个resizeevent
void imgWidget::resizeEvent(QResizeEvent *e){
        if(displayView->scaleFlag == 0&& (!allImgs.isEmpty())){//没有滚轮缩放，可以跟窗口缩放
            displayView->thread->exit(0);
            displayView->thread->widWidth = width()*1.0;
            displayView->thread->widHeight = height()*1.0;
            displayView->timer->start(100);
            displayView->item->setTransformOriginPoint(this->width()/2,this->height()/2);
            //displayView->item->setPos((this->width()-img.width())/2.0,(this->height()-img.height())/2.0);
            //qDebug()<<"TransformOriginPoint"<<displayView->item->transformOriginPoint();
        }
        displayView->setGeometry(0,0,this->width(),this->height());
        displayView->item->setPos((this->width()-displayView->img.width())/2.0,(this->height()-displayView->img.height())/2.0);
        displayView->item->setTransformOriginPoint(this->width()/2,this->height()/2);
       // qDebug()<<"displayView->item"<<displayView->item->pos();\
        //qDebug()<<"displayView"<<displayView->pos()<<displayView->size();
        thumb_view->setGeometry(0,this->height()-81,this->width(),48);
        thumb_view->scene.setSceneRect(0,this->height()-81,this->width(),48);
        thumb_view->widwidth = this->width();
        thumb_view->widheight = this->height();
        for(int i=0;i<thumb_view->imgNum;i++){
            thumb_view->setItemPos(i);
        }
}



