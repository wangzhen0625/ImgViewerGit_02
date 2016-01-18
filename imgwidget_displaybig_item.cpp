#include "imgwidget_displaybig_item.h"

imgWidgetDisplayBigItem::imgWidgetDisplayBigItem()
{
    scaleNum = 10;
}

void imgWidgetDisplayBigItem::wheelEvent(QWheelEvent *e){
    int numDegrees = e->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离
    int numSteps = numDegrees / 15;//滚动的步数，*15就是鼠标滚动的角度
    if (e->orientation() == Qt::Vertical) {
        //qDebug()<<"垂直滚动 = "<<numSteps;//垂直滚动
        scaleNum+=numSteps;
        if(scaleNum<0){
            scaleNum++;
        }else if(scaleNum>30){
            //scaleNum--;
        }
        qDebug()<<e->x()<<e->y();
        setTransformOriginPoint(e->x(),e->y());
        setScale(scaleNum/5.0);
    } else {
     qDebug()<<"水平滚动 = ";      //水平滚动
    }
    e->accept();      //接收该事件
}

 void imgWidgetDisplayBigItem::enterEvent(QEvent *e){
    qDebug()<<"ItemShow::enterEvent";
 }
