#include "imgwidget_thumbthread.h"

imgwidgetThumbThread::imgwidgetThumbThread()
{
    saveAsFlag = 0;
    breakNum = 0;
    selectedNum =0;
    imgNum = 0;
}

Image::Image(){

}

//x：y：图片在背景的位置。sx：图片被使用的起始位置。w：图片被使用的宽度
void Image::painter(QImage image,int x,int y,int w,int h){
    QRectF target(x,y, w, h);//目标文件位置
    QRectF source(0, 0.0, w, h);
    QPainter painter(this);
    painter.drawImage(target, image,source);

}

void imgwidgetThumbThread::setimg(int i){

    float multiple = 0;//倍数
    QImage img;
    Image imgFlag72;
    imgFlag72.load("../bkbig.png");
    double tim = static_cast<double>(getTickCount());
    QImageReader * imgread = new QImageReader(allImgs.at(i).absoluteFilePath());
    tim = ((double)getTickCount()- tim)/getTickFrequency();
    //qDebug()<<"tim----------------"<<tim;
    if(!imgread->canRead()){
       imgread->setFileName("../picture_error.png");
    }
    QSize size = imgread->size();
    multiple = (imgread->size().width()/72.0>imgread->size().height()/48.0
                ?imgread->size().width()/72.0:imgread->size().height()/48.0);
    if(multiple>0)
        imgread->setScaledSize(imgread->size()/multiple);
     int x = (72-imgread->scaledSize().width())/2.0;
     int y = (48-imgread->scaledSize().height())/2.0;

    if(imgread->read(&img))
    {
        imgFlag72.painter(img,x,y,72,48);
        double tim = static_cast<double>(getTickCount());
        //imgFlag36 = imgFlag72.copy(18,0,36,48);
//        tim = ((double)getTickCount()- tim)/getTickFrequency();
//        qDebug()<<"tim----------------"<<tim<<i<<selectedNum;
        emit sendThumb(imgFlag72,img,size,i);
    }
}

void imgwidgetThumbThread::run(){

if(saveAsFlag == 1){
    double tim = static_cast<double>(getTickCount());
    breakNum = 0;
     Mat img = imread(allImgs.at(selectedNum).absoluteFilePath().toStdString(),-1);//
     imwrite(filename.toStdString(),img);
     tim = ((double)getTickCount()- tim)/getTickFrequency();
     //qDebug()<<"tim----------------"<<tim<<filename;
    saveAsFlag =0;
}else{
        setimg(selectedNum);
        int pre,next;
        for(int i = 1;i<imgNum/2+1;i++){
           pre = selectedNum - i;
           if(pre<0)
               pre = allImgs.length()+pre;
           next = selectedNum +i;
           if(next >allImgs.length()-1)
               next =next - allImgs.length();
           setimg(pre);
           setimg(next);
           if(breakNum == 1){
                   breakNum = 0;
                   break;
            }
        }
    }
}
