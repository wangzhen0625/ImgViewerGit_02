#ifndef IMGWIDGETTHUMBTHREAD_H
#define IMGWIDGETTHUMBTHREAD_H
#include <QThread>
#include <QDir>
#include <QImage>
#include <QPainter>
#include <QFileInfo>
#include <QImageReader>
#include <QImageWriter>
#include <QDebug>
#include <opencv.hpp>
using namespace cv;
class imgwidgetThumbThread : public QThread
{
    Q_OBJECT
public:
    void setimg(int i);
    imgwidgetThumbThread();
    void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
                                            cv::Mat &output, cv::Point2i location);
    bool removeFolderContent(const QString &folderDir);
    QFileInfoList allImgs;
    int selectedNum;
    int  imgNum;//文件总数
    int saveAsFlag;
    int breakNum;
    QString filename;
signals:
    void sendThumb(QImage fullImg,QImage halfImg,QSize size,int pos);
protected:
    void run();
};
class Image:public  QImage{
public:
    Image();
    void painter(QImage img, int x, int y, int w, int h);
};

#endif // IMGWIDGETTHUMBTHREAD_H
