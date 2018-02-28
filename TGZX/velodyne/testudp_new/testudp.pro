#-------------------------------------------------
#
# Project created by QtCreator 2018-02-24T09:27:51
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testudp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/pcl-1.8 \
               /usr/include/eigen3 \
              # /usr/include/openni2 \
               /usr/include/vtk-5.10 \
               /usr/include/boost \
               /usr/include/flann \
               /usr/include/qhull


LIBS +="-L/usr/local/lib"
LIBS +="-L/usr/lib"
LIBS +=-lboost_system

LIBS += -lpcl_apps \
        -lpcl_common \
        -lpcl_filters \
        -lpcl_keypoints \
        -lpcl_kdtree \
        -lpcl_search \
        -lpcl_features \
        -lpcl_io \
        -lpcl_io_ply \
        -lpcl_visualization \
        -lpcl_sample_consensus \
        -lpcl_segmentation \
        -lpcl_recognition \
        -lpcl_registration

LIBS += -lvtkCommon \
        -lvtkGraphics \
        -lvtkFiltering \
        -lvtkIO \
        -lvtkImaging \
        -lvtkRendering \
        -lQVTK
