#ifndef IMGWIDGETDISPLAYTHREAD_H
#define IMGWIDGETDISPLAYTHREAD_H
#include <QThread>
#include <QImageReader>
#include <QImage>
#include <QDebug>

#include <QDir>

#include <QPainter>
#include <QFileInfo>

#include <QImageWriter>

class imgWidgetDisplayThread : public QThread
{
    Q_OBJECT
public:
    imgWidgetDisplayThread();
    QString fileName;


    int breakNum;
    float widWidth;
    float widHeight;
signals:
    void sendThumb(QImage image,QString filename);
protected:
    void run(){//传送QImagereader其实不比传送QImage快。所以还是传送QImage吧。毕竟是面向IO的
        float multiple = 0;
       // breakNum = 1;
        QString file = fileName;
        qDebug()<<"thread"<<fileName;

        QImage img;
        QImageReader *imgRead = new QImageReader(fileName);
        if(!imgRead->canRead()){
           //qDebug()<<"QImageReader * imgread errorrrrr";
           imgRead->setFileName("../picture_error.png");
        }
        multiple= (imgRead->size().width()/widWidth>imgRead->size().height()/widHeight
                    ?imgRead->size().width()/widWidth:imgRead->size().height()/widHeight);
        if(multiple>1){
            imgRead->setScaledSize(imgRead->size()/multiple);
        }
        if(imgRead->read(&img)){//这里消耗时间最多
            emit sendThumb(img,file);
        }

    }
};

#endif // IMGWIDGETDISPLAYTHREAD_H
