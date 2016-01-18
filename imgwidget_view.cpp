#include "imgwidget_view.h"

imgwidgetview::imgwidgetview(QWidget *parent):QGraphicsView(parent)
{
    wheelFlag = 0;
    selectedNum = 0;
    oldSelectNum = 0;
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setStyleSheet("background:transparent;border:0px");
    this->setScene(&scene);
    this->setFixedHeight(48);
    //scene.setSceneRect(0,widheight-81,widwidth,48);
    ThumbThread = new imgwidgetThumbThread;
    saveAsThread = new imgwidgetThumbThread;
    connect(ThumbThread,SIGNAL(sendThumb(QImage,QImage,QSize,int)),this,SLOT(itemAddImg(QImage,QImage,QSize,int)));


}

/******************************************
1.从imgwidget获取到allimgs后，给secene添加item，
并且把allimgs传给线程对象1,让他创建半张和整张缩略图。
为下一步item添加图片做基础
*******************************************/
void imgwidgetview::setItem(){
    //1.重新加载设置item和图片，所以需要清空
    if(!allPixItem.isEmpty()){
        scene.clear();
        allAnimation.clear();
        midPixItems.clear();
        allPixItem.clear();
        all_Qimg.clear();
        all_size.clear();
    }
    emit sendToDisplay(currentfile);

    //keepSameTimer->start(3000);//1000毫秒 = 1s
    //2.判断当前选中的文件在allimgs中的序号，作为当前选中序号
    for(int i = 0; i < imgNum; ++i) {
        if(!(allImgs.at(i).fileName().compare(QFileInfo(currentfile).fileName())))//qt比较字符串 当前选中的文件序号
        {
            selectedNum=i;//在文件选择框选择的文件在列表的序号，用来在缩略图中当前显示
            oldSelectNum =i;
        }
   }
    //3.缩略图半张图的item准备好,并添加到scene,位移的变量也准备好
    for(int i =0;i<imgNum;++i)
    {
        QGraphicsPixmapItem *pixItemhalf = new QGraphicsPixmapItem;
        QGraphicsPixmapItem *pixItemfull = new QGraphicsPixmapItem;
        allPixItem.append(pixItemhalf);
        midPixItems.append(pixItemfull);
        QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
        scene.addItem(allPixItem.at(i));
        allAnimation.append(animation);
    }
    //4.保存中间的全张缩略图的item--imgNum也准备好，并且添加到scene，位移变量也准备好
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
    midPixItemFlag = midPixItems.at(selectedNum);
    scene.addItem(midPixItemFlag);
    allAnimation.append(animation);
    allAnimation.at(imgNum)->setItem(midPixItemFlag);
    //5.allimgs传给线程对象1，让他创建半张和全张缩略图，并传递过来
    if(ThumbThread->isRunning()){
        ThumbThread->breakNum = 1;
        ThumbThread->wait(500);
    }
    ThumbThread->selectedNum = selectedNum;
    ThumbThread->allImgs = allImgs;
    ThumbThread->imgNum = imgNum;
    ThumbThread->start();
}
/**********************************
* 2.接收线程发送过来的半张和全张缩略图  *
* 分别添加到midpixitem和allpixitem *
***********************************/
void imgwidgetview::itemAddImg(QImage fullImg, QImage img, QSize size, int i){
    all_Qimg[i] = img;
    all_size[i] = size;
    QImage halfImg = fullImg.copy(18,0,36,48);
    midPixItems.at(i)->setPixmap(QPixmap::fromImage(fullImg));
    allPixItem.at(i)->setPixmap(QPixmap::fromImage(halfImg));
    allPixItem.at(i)->setData(1001,allImgs.at(i).absoluteFilePath());
    allPixItem.at(i)->setToolTip(
                "<span style=\"color: white; background-color: transparent;\">"
                                   + allImgs.at(i).fileName() + "</span>");
    midPixItems.at(i)->setToolTip(
                "<span style=\"color: white; background-color:transparent;\">"
                                   + allImgs.at(i).fileName() + "</span>");
    setItemPos(i);
}
/**********************************
 *      动态设置item的位置          *
 *  窗口改变时需要动态传递进来窗口大小 *
 *           并改变位置            *
**********************************/
void imgwidgetview::setItemPos(int i)
{
    if(i<selectedNum){//left  --
        //将该项目的位置设置为给定的步长值到指定的点。
       allPixItem.at(i)->setPos((36+1)*(i-selectedNum-1)+width()/2.0,widheight-81);
    }else if(i>selectedNum){//right  ++
        //将该项目的位置设置为给定的步长值到指定的点。
        allPixItem.at(i)->setPos(3+(36+1)*(i-selectedNum)+width()/2.0,widheight-81);
    }else{//selected
        //将该项目的位置设置为给定的步长值到指定的点。
        midPixItemFlag->setPos(width()/2.0+2-36,widheight-81);
        allPixItem.at(i)->setPos(width()/2.0+2-18,widheight-81);
        midPixItemFlag->show();
        allPixItem.at(i)->show();
        allPixItem.at(i)->hide();
    }
}
void imgwidgetview::movItemPos(int i){
    allPixItem.at(i)->show();
    QTimeLine *timeLine = new QTimeLine(500);
    timeLine->setFrameRange(0, 500);
    //midPixItemFlag->hide();
    scene.setSceneRect(0,widheight-81,widwidth,48);
    //scene.setSceneRect(-widwidth/2, -24,widwidth,48);
    allAnimation.at(i)->setTimeLine(timeLine);
    allAnimation.at(i)->clear();
    allAnimation.at(i)->setItem(allPixItem.at(i));
    if(i<selectedNum){//left
        //将该项目的位置设置为给定的步长值到指定的点。
        allAnimation.at(i)->setPosAt(1, QPointF((36+1)*(i-selectedNum-1)+width()/2.0,widheight-81));
    }else if(i>selectedNum){//right
        //将该项目的位置设置为给定的步长值到指定的点。
        allAnimation.at(i)->setPosAt(1, QPointF(3+(36+1)*(i-selectedNum)+width()/2.0,widheight-81));
    }else{//selected
        if(selectedNum!=oldSelectNum){

            midPixItemFlag->hide();
            oldSelectNum = selectedNum;
        }
        //将该项目的位置设置为给定的步长值到指定的点。
        allAnimation.at(i)->setPosAt(1, QPointF(3-18+(36+1)*(i-selectedNum)+width()/2.0,widheight-81));
        connect(timeLine,SIGNAL(finished()),this,SLOT(timelineFinished()));
    }
    timeLine->start();
}

void imgwidgetview::timelineFinished(){
    scene.removeItem(midPixItemFlag);
    midPixItemFlag = midPixItems.at(selectedNum);
    scene.addItem(midPixItemFlag);
    midPixItemFlag->setPos(width()/2.0+2-36,widheight-81);
    midPixItemFlag->show();
    allPixItem.at(selectedNum)->hide();

}

void imgwidgetview::changeSelectNum(){
    for(int i=0;i<imgNum;i++){
        movItemPos(i);
    }
}

void imgwidgetview::preImg(int num){
    if(!allImgs.isEmpty()){
        qDebug()<<"num"<<num;
    }else{
        QMessageBox message(QMessageBox::NoIcon, "提示", "请先选择图片文件!");
        message.setIconPixmap(QPixmap("../2.png"));
        message.exec();
    }
}

void imgwidgetview::nextImg(int num){
    if(!allImgs.isEmpty()){

    }else{
        QMessageBox message(QMessageBox::NoIcon, "提示", "请先选择图片文件!");
        message.setIconPixmap(QPixmap("../2.png"));
        message.exec();
    }
}

void imgwidgetview::mousePressEvent(QMouseEvent *e){
    if(!allImgs.isEmpty()){
        currentfile = this->itemAt(e->pos())->data(1001).toString();
        if(currentfile!=""){        
            for(int i=0;i<imgNum;i++){
                if(!allImgs.at(i).absoluteFilePath().compare(currentfile))
                    selectedNum = i;
            }
            //qDebug()<<"selectednum"<<selectedNum<<currentfile;
        }
        if(selectedNum!=oldSelectNum){
            if(wheelFlag == 0){
                QTimer *timer = new QTimer(this);
                connect(timer, SIGNAL(timeout()), this, SLOT(wheelChangeImg()));
                timer->start(300);
                timer->setSingleShot(true);//once
            }
            wheelFlag = 1;
            //changeSelectNum();
            emit sendToDisplayImg(all_Qimg[selectedNum],all_size[selectedNum]);
        }
    }else{
        QMessageBox message(QMessageBox::NoIcon, "提示", "请先选择图片文件!");
        message.setIconPixmap(QPixmap("../2.png"));
        message.exec();
    }
}

void imgwidgetview::wheelChangeImg(){
    emit sendToDisplay(currentfile);
    changeSelectNum();
    wheelFlag = 0;
}

void imgwidgetview::wheelEvent(QWheelEvent *e){
    if(!allImgs.isEmpty()){        
        if (e->orientation() == Qt::Vertical) {
            if(wheelFlag == 0){
                QTimer *timer = new QTimer(this);
                connect(timer, SIGNAL(timeout()), this, SLOT(wheelChangeImg()));
                timer->start(400);
                timer->setSingleShot(true);//once
            }
            int numDegrees = e->delta() / 8;//滚动的角度，*8就是鼠标滚动的距离
            int numSteps = numDegrees / 15;//滚动的步数，*15就是鼠标滚动的角度
            wheelFlag += numSteps;
            //selectedNum-=wheelFlag;
            selectedNum -= numSteps;
            if(selectedNum<0){
                selectedNum = imgNum-1;
            }else if(selectedNum>=imgNum){
                selectedNum = 0;
            }
           // qDebug()<<"垂直滚动 = "<<numSteps;//垂直滚动
            currentfile = allPixItem.at(selectedNum)->data(1001).toString();
            emit sendToDisplayImg(all_Qimg[selectedNum],all_size[selectedNum]);
        } else {
         qDebug()<<"水平滚动 = ";      //水平滚动
        }
        e->accept();      //接收该事件
    }else{
        QMessageBox message(QMessageBox::NoIcon, "提示", "请先选择图片文件!");
        message.setIconPixmap(QPixmap("../2.png"));
        message.exec();
    }
}

void imgwidgetview::playSlide(){
    qDebug()<<"imgwidgetview-playSlide";
    selectedNum++;
    if(selectedNum<0){
        selectedNum = imgNum-1;
    }else if(selectedNum>=imgNum){
        selectedNum = 0;
    }
    currentfile = allPixItem.at(selectedNum)->data(1001).toString();
    emit sendToDisplay(currentfile);
    changeSelectNum();
}

void imgwidgetview::removeFile(){
    if(!allImgs.isEmpty()){
        if(imgNum>0){
            qDebug()<<"removeFile1";
            QFile::remove(allImgs.at(selectedNum).absoluteFilePath());
            allImgs.removeAt(selectedNum);
            allAnimation.removeAt(selectedNum);
            scene.removeItem(midPixItems.at(selectedNum));
            scene.removeItem(allPixItem.at(selectedNum));
            midPixItems.removeAt(selectedNum);
            allPixItem.removeAt(selectedNum);
            if(selectedNum>=imgNum-1)
                selectedNum=0;
            imgNum--;
            oldSelectNum = selectedNum;
            if(imgNum>0){
                currentfile = allImgs.at(selectedNum).absoluteFilePath();
                emit sendToDisplay(currentfile);
                 changeSelectNum();
                 qDebug()<<"removeFile4"<<selectedNum<<imgNum;
            }
        }
    }else{
        QMessageBox message(QMessageBox::NoIcon, "提示", "请先选择图片文件!");
        message.setIconPixmap(QPixmap("../2.png"));
        message.exec();
    }



}



void imgwidgetview::copyFile(){
    if(!allImgs.isEmpty()){
        QPixmap pixmap;
        pixmap.load(allImgs.at(selectedNum).absoluteFilePath());
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setPixmap(pixmap);
    }else{
        QMessageBox message(QMessageBox::NoIcon, "提示", "请先选择图片文件!");
        message.setIconPixmap(QPixmap("../2.png"));
        message.exec();
    }

}

void imgwidgetview::saveAs(){
    if(!allImgs.isEmpty()){
        QFileDialog *fileDialog = new QFileDialog(this);//创建一个QFileDialog对象，构造函数中的参数可以有所添加。
        fileDialog->setWindowTitle(tr("Save As"));//设置文件保存对话框的标题
        fileDialog->setAcceptMode(QFileDialog::AcceptSave);//设置文件对话框为保存模式
        fileDialog->setFileMode(QFileDialog::AnyFile);//设置文件对话框弹出的时候显示任何文件，不论是文件夹还是文件
        fileDialog->setViewMode(QFileDialog::Detail);//文件以详细的形式显示，显示文件名，大小，创建日期等信息；
        QString filename = fileDialog->getSaveFileName(this, tr("Save File"), "/home/wz/img/未命名.png", tr("Images (*.png)"));
        qDebug()<<filename<<currentfile;
        fileDialog->setGeometry(10,30,100,200);//设置文件对话框的显示位置

        saveAsThread->saveAsFlag = 1;
        saveAsThread->breakNum = 1;
        if(!saveAsThread->allImgs.isEmpty()){
            saveAsThread->allImgs.clear();
        }
        saveAsThread->allImgs = allImgs;
        saveAsThread->imgNum = imgNum;
        saveAsThread->filename = filename;
        saveAsThread->selectedNum = selectedNum;
        saveAsThread->start();
    }else{
        QMessageBox message(QMessageBox::NoIcon, "提示", "请先选择图片文件!");
        message.setIconPixmap(QPixmap("../2.png"));
        message.exec();
    }

}

void imgwidgetview::setAsWallpaper(){
    if(!allImgs.isEmpty()){
        QProcess *proc = new QProcess;
        QString str = "gsettings set org.gnome.desktop.background picture-uri 'file://"
                +allImgs.at(selectedNum).absoluteFilePath()+"'";
        proc->start(str);
    }else{
        QMessageBox message(QMessageBox::NoIcon, "提示", "请先选择图片文件!");
        message.setIconPixmap(QPixmap("../2.png"));
        message.exec();
    }
}
