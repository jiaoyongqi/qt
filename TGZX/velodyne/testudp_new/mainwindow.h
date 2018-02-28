#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QtMath>
#include <QtNetwork>
#include <QUdpSocket>
#include <QApplication>
#include <QHostAddress>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/conditional_removal.h>

typedef struct _LidarInfo
{
  float Azimuth;//方位角
  float ChannelDis[16];//通道距离
}LidarInfo;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static volatile int savefile;


    static volatile double Cosdu[16];
    static volatile double Sindu[16];
    void InitLidar();
    void InitSlot();
    void InitTimer();
    void ExtractLidarMsg();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
  void NextcTimerSlot();
  void ReceiverLidarDataSlot();
  void ProcessLidarDataSlot();


signals:
  void ProcessLidarData();

private:
    Ui::MainWindow *ui;
    //激光雷达数据
    QList<QByteArray> qLidarItems;//激光雷达集合
    QUdpSocket *qLidarSocket;
    QList<LidarInfo> qLidarList;
    QList<LidarInfo> qLidarList_extract;
    pcl::PointCloud<pcl::PointXYZ> cloud;

    //计数timer
    QTimer *cTimers;
    volatile int Li;
    volatile int QLidarListStatus;
    volatile int QLidarListCopyStatus;

};

#endif // MAINWINDOW_H
