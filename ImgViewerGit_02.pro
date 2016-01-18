#-------------------------------------------------
#
# Project created by QtCreator 2015-12-30T13:33:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImgViewer_02
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    imgwidget.cpp \
    imgwidget_thumbthread.cpp \
    imgwidget_view.cpp \
    imgwidget_displaybig_item.cpp \
    imgwidget_displaybig_view.cpp \
    imgwidget_display_thread.cpp

HEADERS  += widget.h \
    imgwidget.h \
    imgwidget_thumbthread.h \
    imgwidget_view.h \
    imgwidget_displaybig_item.h \
    imgwidget_displaybig_view.h \
    imgwidget_display_thread.h


INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

LIBS +=/usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so  \
        /usr/local/lib/libopencv_imgcodecs.so
